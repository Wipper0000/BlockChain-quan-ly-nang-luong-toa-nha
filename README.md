
<h2 align="center">
    <a href="https://dainam.edu.vn/vi/khoa-cong-nghe-thong-tin">
    🎓 Faculty of Information Technology (DaiNam University)
    </a>
</h2>
<h2 align="center">
   Ứng dụng BlockChain trong quản lý năng lượng tòa nhà
</h2>
<div align="center">
    <p align="center">
        <img src="docs/aiotlab_logo.png" alt="AIoTLab Logo" width="170"/>
        <img src="docs/fitdnu_logo.png" alt="AIoTLab Logo" width="180"/>
        <img src="docs/dnu_logo.png" alt="DaiNam University Logo" width="200"/>
    </p>

[![AIoTLab](https://img.shields.io/badge/AIoTLab-green?style=for-the-badge)](https://www.facebook.com/DNUAIoTLab)
[![Faculty of Information Technology](https://img.shields.io/badge/Faculty%20of%20Information%20Technology-blue?style=for-the-badge)](https://dainam.edu.vn/vi/khoa-cong-nghe-thong-tin)
[![DaiNam University](https://img.shields.io/badge/DaiNam%20University-orange?style=for-the-badge)](https://dainam.edu.vn)

</div>

## 📖 1. Giới thiệu
Trong thời đại công nghệ thông tin phát triển mạnh mẽ, nhu cầu trao đổi và chia sẻ dữ liệu giữa các máy tính ngày càng phổ biến. Một trong những phương thức quan trọng và được sử dụng rộng rãi là truyền file qua giao thức TCP (Transmission Control Protocol).

TCP là giao thức hướng kết nối, đảm bảo tính tin cậy, toàn vẹn và đúng thứ tự dữ liệu trong quá trình truyền. Nhờ đó, việc truyền file qua TCP có thể áp dụng cho nhiều hệ thống thực tế như: dịch vụ FTP, ứng dụng chat, hệ thống lưu trữ dữ liệu phân tán, hay các phần mềm hỗ trợ tải xuống.

🔑 **Các chức năng chính của hệ thống**:

-Kết nối Client – Server qua TCP Socket.

-Truyền và nhận file giữa các client.

-Hiển thị tiến trình truyền và nhận file .

-Lưu file tự động vào thư mục chỉ định.

## 🛠️ 2. Công nghệ sử dụng

**Ngôn ngữ lập trình**: Java

**Giao thức**: TCP,TCP Socket

**Công nghệ giao diện**: Java Swing

**Thư viện**:

-java.net 

-java.io 

**Công cụ phát triển**: Eclipse IDE

**Hệ điều hành**: Windows 11

**JDK**: Java SE


## 🚀 3. Hình ảnh các chức năng


<p align="center">
  <img src="docs/Ảnh 1.png" alt="Ảnh 1" width="500"/>
</p>
<p align="center">
  <em>Hình 1: Giao diện nhập tên user </em>
</p>


<p align="center">
  <img src="docs/Ảnh 3.png" alt="Ảnh 3" width="500"/>
</p>
<p align="center">
  <em>Hình 2: Giao diện chọn file để gửi </em>
</p>


<p align="center">
  <img src="docs/Ảnh 4.png" alt="Ảnh 4" width="500"/>
</p>
<p align="center">
  <em>Hình 3: Giao diện Client sau khi chọn  </em>
</p>


<p align="center">
  <img src="docs/Ảnh 5.png" alt="Ảnh 5" width="500"/>
</p>
<p align="center">
  <em>Hình 4: Giao diện Khi File gửi thành công  </em>
</p>


<p align="center">
  <img src="docs/Ảnh 6.png" alt="Ảnh 6" width="500"/>
</p>
<p align="center">
  <em>Hình 5: Giao diện Client user khác truy cập server sau khi user 1 chuyển file</em>
</p>

## 📝 4. Các bước cài đặt

#### Bước 1: Chuẩn bị môi trường
1. **Kiểm tra Java**: Mở terminal/command prompt và chạy:
   ```bash
   java -version
   javac -version
   ```
   Đảm bảo cả hai lệnh đều hiển thị phiên bản Java 8 trở lên.

2. **Tải mã nguồn**: Sao chép thư mục `Truyen-file-qua-tcp` chứa các file:
   - `FileServer.java`
   - `ChatClient.java`

#### Bước 2: Biên dịch mã nguồn
1.Chạy server:
```bash
java component.FileServer
```

2.Chạy Client:
```bash
java component.ChatClient
```
<p></p>

### 🚀 Sử dụng ứng dụng

1. **Kết nối server**: chạy Server.java để lắng nghe trên port (ví dụ 5001).
2. **Kết nối Client**: chạy Client.java và nhập IP, port server để kết nối.
3. **Chọn File**: client chọn file cần gửi từ máy tính.
4. **Gửi File**: client gửi file qua socket TCP tới server.
5. **Nhận File**: server nhận dữ liệu và ghi file vào thư mục đích.
6. **Lưu File**: file được lưu tự động trong thư mục mặc định của server.
7. **Ngắt kết nối**:client thoát ứng dụng hoặc nhấn Ctrl+C để dừng.

## 👤 5. Liên hệ
**Họ tên**: Lê Ngọc Hưng.  
**Lớp**: CNTT 16-01.  
**Email**: Hungtit0601@gmail.com.

© 2025 AIoTLab, Faculty of Information Technology, DaiNam University. All rights reserved.

---
