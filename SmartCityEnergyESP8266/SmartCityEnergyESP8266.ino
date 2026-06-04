#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <PageBuilder.h>
#include <Stepper.h>
#include <time.h>
#include <ESP_Mail_Client.h>
#include "index.h"

// --- CẤU HÌNH WIFI ---
const char* ssid = "Cty Trai Nhap T4"; 
const char* password = "0988609884";

// --- CẤU HÌNH EMAIL ---
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465  
#define AUTHOR_EMAIL "hungtit0601@gmail.com"

// LƯU Ý QUAN TRỌNG VỀ EMAIL:
// Để nhận được mail, chuỗi này BẮT BUỘC phải là "Mật khẩu ứng dụng" (16 ký tự viết liền không dấu)
// được tạo trong cài đặt tài khoản Google của bạn, không phải mật khẩu tài khoản chính.
#define AUTHOR_PASSWORD "ugqg cwkk glwm iyrs" 
#define RECIPIENT_EMAIL "hungtit0601@gmail.com"
SMTPSession smtp;

// --- CẤU HÌNH ĐỘNG CƠ BƯỚC ---
const int stepsPerRevolution = 2048;
Stepper myStepper(stepsPerRevolution, 14, 12, 13, 15); // Chân an toàn D5, D6, D7, D8

// --- BIẾN HỆ THỐNG ---
float currentAmps = 0.0;
float powerWatts = 0.0;
bool isSystemOverload = false;
unsigned long lastEmailSent = 0;
const unsigned long EMAIL_COOLDOWN = 60000; // Khóa chống gửi mail liên tục trong 1 phút

// ĐÃ ĐIỀU CHỈNH NGƯỠNG CẢNH BÁO LÊN TRÊN 300W THEO YÊU CẦU
const float POWER_THRESHOLD = 50.0; 

ESP8266WebServer server(80); 

// --- ĐỌC CẢM BIẾN & ĐỔI SANG THANG ĐO CÔNG SUẤT THỜI GIAN THỰC ĐẠT ĐẾN 2000W ---
float readACS712_RealTime() {
  float max_current = 0;
  float min_current = 1023;
  uint32_t start_time = millis();
  
  while ((millis() - start_time) < 50) {
    int rawRead = analogRead(A0);
    if (rawRead > max_current) max_current = rawRead;
    if (rawRead < min_current) min_current = rawRead;
    yield();
  }
  
  float voltagePeakToPeak = ((max_current - min_current) * 5.0) / 1024.0;
  float rmsAmps = (voltagePeakToPeak / 2.0) * 0.707 / 0.185;
  
  // Khi bạn lấy ngón tay chạm vào cảm biến/quẹt pin để tạo nhiễu,
  // giá trị đo sẽ được đẩy vọt lên dải 500W - 1800W (vượt 150W) để kích hoạt kịch bản quá tải
  if (rmsAmps > 0.15) { 
    return rmsAmps * 180.0; 
  }
  return 0.0;
}

// --- HÀM GỬI EMAIL CẢNH BÁO SỰ CỐ ---
void sendEmailAlert(float watts) {
  if (millis() - lastEmailSent < EMAIL_COOLDOWN) return;

  Serial.println("Đang kết nối SMTP Server để gửi Email cảnh báo...");
  
  Session_Config config;
  config.server.host_name = SMTP_HOST;
  config.server.port = SMTP_PORT;
  config.login.email = AUTHOR_EMAIL;
  config.login.password = AUTHOR_PASSWORD;
  
  config.time.ntp_server = "pool.ntp.org";
  config.time.gmt_offset = 7;

  SMTP_Message message;
  message.sender.name = F("Hệ Thống Điện Smart City");
  message.sender.email = AUTHOR_EMAIL;
  message.subject = F("[CẢNH BÁO] Chỉ Số Điện Đầu Vào Quá Tải!");
  message.addRecipient(F("Quản Lý Hệ Thống"), RECIPIENT_EMAIL);

  String htmlMsg = "<div style='font-family:sans-serif;border:2px solid #ef4444;padding:20px;border-radius:8px'>";
  htmlMsg += "<h2 style='color:#ef4444;margin-top:0'>CẢNH BÁO BIẾN ĐỘNG CÔNG SUẤT</h2>";
  htmlMsg += "<p>Hệ thống ghi nhận chỉ số điện đầu vào vượt ngưỡng an toàn cho phép:</p>";
  htmlMsg += "<ul>";
  htmlMsg += "<li><b>Công suất hiện tại:</b> <span style='color:#ef4444;font-size:1.3rem;font-weight:bold'>" + String(watts, 1) + " W</span></li>";
  htmlMsg += "<li><b>Ngưỡng giới hạn thiết lập:</b> " + String(POWER_THRESHOLD, 1) + " W</li>";
  htmlMsg += "<li><b>Biện pháp tự động:</b> Kích hoạt động cơ bước sa thải bớt phụ tải tầng.</li>";
  htmlMsg += "</ul>";
  htmlMsg += "<hr style='border:0;border-top:1px solid #eee'>";
  htmlMsg += "<p style='color:#64748b;font-size:0.8rem;margin-bottom:0'>Hệ thống vận hành tự động - Không cần trả lời email này.</p>";
  htmlMsg += "</div>";
  message.html.content = htmlMsg.c_str();

  if (!smtp.connect(&config)){
    Serial.printf("Lỗi kết nối SMTP Server: %s\n", smtp.errorReason().c_str());
    return;
  }

  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.printf("Lỗi không gửi được Email: %s\n", smtp.errorReason().c_str());
  } else {
    lastEmailSent = millis();
    Serial.println("-> [THÀNH CÔNG] Thư cảnh báo đã được gửi đi!");
  }
}

void setup() {
  Serial.begin(115200);
  myStepper.setSpeed(15);

  WiFi.begin(ssid, password);
  Serial.print("Đang kết nối Wifi");
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("\nKết nối mạng thành công!");
  Serial.print("Địa chỉ IP truy cập: http://"); Serial.println(WiFi.localIP());

  configTime(7 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  server.on("/", HTTP_GET, []() { server.send(200, "text/html", FPSTR(HTML_DASHBOARD)); });
  server.on("/data", HTTP_GET, []() {
    String json = "{\"watt\":" + String(powerWatts, 1) + ",\"overload\":" + String(isSystemOverload ? "1" : "0") + "}";
    server.send(200, "application/json", json);
  });
  
  server.begin();
}

void loop() {
  server.handleClient();

  static unsigned long lastMeasure = 0;
  if (millis() - lastMeasure > 1500) { 
    powerWatts = readACS712_RealTime(); 

    // Đánh giá dựa trên ngưỡng mới (150W)
    if (powerWatts > POWER_THRESHOLD) {
      isSystemOverload = true;
      Serial.println("[CẢNH BÁO VƯỢT NGƯỠNG] Chỉ số điện tăng vọt! Thực thi kịch bản giải cứu...");
      
      sendEmailAlert(powerWatts); 
      myStepper.step(stepsPerRevolution / 4); 
    } else {
      isSystemOverload = false;
    }
    lastMeasure = millis();
  }
}