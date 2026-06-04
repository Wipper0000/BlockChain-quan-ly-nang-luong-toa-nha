#ifndef INDEX_H
#define INDEX_H

const char HTML_DASHBOARD[] PROGMEM = R"ENDHTML(
<!DOCTYPE html>
<html lang="vi">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width,initial-scale=1">
    <title>Hệ Thống Năng Lượng Blockchain Smart City</title>
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/web3@1.8.1/dist/web3.min.js"></script>
    
    <style>
        body{background:#0f172a;color:#f8fafc;font-family:sans-serif;text-align:center;padding:20px;margin:0}
        .container{max-width:600px;margin:auto;padding:10px}
        .box{border:1px solid #38bdf8;padding:20px;border-radius:10px;background:#1e293b;margin-bottom:20px;box-shadow:0 4px 6px -1px rgba(0,0,0,0.1)}
        .status{padding:10px;border-radius:5px;margin-top:10px;font-weight:bold;transition:0.3s;letter-spacing:1px}
        .chart-container{height:250px;background:#1e293b;border:1px solid #334155;border-radius:10px;padding:12px;margin-bottom:20px}
        .table-container{background:#1e293b;border:1px solid #334155;border-radius:10px;padding:15px;text-align:left}
        table{width:100%;border-collapse:collapse;margin-top:10px;font-size:0.85rem}
        th,td{padding:8px;text-align:center;border-bottom:1px solid #334155}
        th{background:#38bdf8;color:#0f172a;font-weight:bold}
        tr:hover{background:#334155}
        .block-info{font-size:0.75rem;color:#64748b;font-family:monospace;word-break:break-all}
    </style>
</head>
<body>

<div class="container">
    <div class="box">
        <h2>GIÁM SÁT NĂNG LƯỢNG REAL-TIME</h2>
        <p style="color:#38bdf8;margin-top:-10px;font-weight:bold">Hệ Thống Lưu Trữ Chuỗi Khối Bất Biến</p>
        <hr style="border-color:#334155"><br>
        <p style="margin:0;color:#94a3b8">Chỉ số điện đầu vào đang tiêu thụ:</p>
        <p style="margin:10px 0"><span id="watt" style="font-size:3.5rem;font-weight:bold;color:#38bdf8">0.0</span> <span style="font-size:1.5rem;color:#38bdf8">W</span></p>
        <div id="status" class="status" style="background:#10b981">HỆ THỐNG AN TOÀN</div>
    </div>

    <div class="chart-container">
        <canvas id="energyChart"></canvas>
    </div>

    <div class="table-container">
        <h3 style="margin-top:0;color:#38bdf8;border-left:4px solid #38bdf8;padding-left:10px">Sổ Cái Lưu Trữ Biến Động (Blockchain Ledger)</h3>
        <table>
            <thead>
                <tr>
                    <th>Thời Gian</th>
                    <th>Công Suất</th>
                    <th>Mã Block Hash / TxHash</th>
                </tr>
            </thead>
            <tbody id="historyLog">
                </tbody>
        </table>
    </div>
</div>

<script>
// --- CẤU HÌNH KẾT NỐI GANACHE BLOCKCHAIN ---
// Kết nối trực tiếp tới cổng RPC mạng nội bộ của phần mềm Ganache trên máy tính
const web3 = new Web3(new Web3.providers.HttpProvider("http://127.0.0.1:7545"));

// Lấy địa chỉ ví thứ 1 và ví thứ 2 công khai từ Ganache để làm tài khoản giao dịch giả lập
let walletAddressSender;
let walletAddressReceiver;

web3.eth.getAccounts().then(accounts => {
    walletAddressSender = accounts[0];    // Ví đại diện tòa nhà
    walletAddressReceiver = accounts[1];  // Ví kiểm toán năng lượng
    console.log("Blockchain Connected! Sender:", walletAddressSender);
    loadPastBlocks(); // Khởi chạy nạp lại lịch sử cũ từ Blockchain khi mở trang hoặc F5
});

// --- KHỞI TẠO BIỂU ĐỒ ---
const ctx = document.getElementById('energyChart').getContext('2d');
const energyChart = new Chart(ctx, {
    type: 'line',
    data: {
        labels: [], 
        datasets: [{
            label: 'Công suất đầu vào (W)',
            data: [], 
            borderColor: '#38bdf8',
            backgroundColor: 'rgba(56, 189, 248, 0.05)',
            borderWidth: 2,
            tension: 0.2
        }]
    },
    options: {
        responsive: true,
        maintainAspectRatio: false,
        scales: {
            y: { min: 0, max: 2000, ticks: { color: '#94a3b8' } },
            x: { ticks: { color: '#94a3b8' } }
        }
    }
});

let isOverloadActive = false; 

// KỸ THUẬT BLOCKCHAIN: Gửi một Transaction chứa dữ liệu điện năng dưới dạng mã Hex
function sendToBlockchain(timeStr, wattValue) {
    if (!walletAddressSender) return;

    // Đóng gói thông tin dữ liệu thành một chuỗi văn bản bản ghi
    const logMessage = "OVERLOAD_LOG: Time=" + timeStr + ", Watt=" + Number(wattValue).toFixed(1) + "W";
    // Mã hóa chuỗi văn bản này sang dạng mã Hex để gửi vào trường Data của Block Ethereum
    const hexData = web3.utils.utf8ToHex(logMessage);

    console.log("Đang đào dữ liệu sự cố vào Block mới...");

    web3.eth.sendTransaction({
        from: walletAddressSender,
        to: walletAddressReceiver,
        value: web3.utils.toWei("0.01", "ether"), // Chuyển một lượng gas nhỏ mô phỏng
        data: hexData                             // Gắn dữ liệu số điện bất biến vào đây
    })
    .then(receipt => {
        console.log("Đã đào thành công! Block Number:", receipt.blockNumber);
        
        // Render dòng mới vào bảng ngay khi giao dịch được xác thực thành công
        addTableRow(timeStr, wattValue, receipt.transactionHash);
    })
    .catch(err => console.error("Lỗi giao dịch chuỗi khối:", err));
}

function addTableRow(time, watt, txHash) {
    const tbody = document.getElementById("historyLog");
    const row = document.createElement("tr");
    
    // Rút gọn mã băm dài để hiển thị gọn gàng trên bảng
    const shortHash = txHash.substring(0, 10) + "..." + txHash.substring(txHash.length - 8);
    
    row.innerHTML = `
        <td>${time}</td>
        <td style="color:#ef4444;font-weight:bold">${Number(watt).toFixed(1)} W</td>
        <td class="block-info" title="${txHash}">${shortHash}</td>
    `;
    
    if(tbody.firstChild) tbody.insertBefore(row, tbody.firstChild);
    else tbody.appendChild(row);
}

// HÀM ĐỌC DỮ LIỆU TỪ QUÁ KHỨ (Giúp chống mất dữ liệu khi F5 trình duyệt)
function loadPastBlocks() {
    web3.eth.getBlockNumber().then(latestBlock => {
        // Duyệt quét ngược lại tối đa 20 block cũ nhất để tìm dữ liệu năng lượng đã lưu trước đó
        let startBlock = Math.max(0, latestBlock - 20);
        for (let i = latestBlock; i >= startBlock; i--) {
            web3.eth.getBlock(i, true).then(block => {
                if (block && block.transactions.length > 0) {
                    block.transactions.forEach(tx => {
                        if (tx.input && tx.input !== "0x") {
                            try {
                                // Giải mã ngược từ mã Hex trong Block thành văn bản đọc được
                                const decoded = web3.utils.hexToUtf8(tx.input);
                                if (decoded.startsWith("OVERLOAD_LOG:")) {
                                    // Phân tích cú pháp chuỗi "OVERLOAD_LOG: Time=10:15:00, Watt=185.5W"
                                    const timePart = decoded.split("Time=")[1].split(",")[0];
                                    const wattPart = decoded.split("Watt=")[1].replace("W", "");
                                    
                                    // Đưa dữ liệu lịch sử chuẩn từ Blockchain ra hiển thị lại bảng dữ liệu
                                    addTableRow(timePart, wattPart, tx.hash);
                                }
                            } catch(e) {}
                        }
                    });
                }
            });
        }
    });
}

function updateChartAndTable(timeStr, wattValue, isOverload) {
    energyChart.data.labels.push(timeStr);
    energyChart.data.datasets[0].data.push(wattValue);
    if(energyChart.data.labels.length > 15) {
        energyChart.data.labels.shift();
        energyChart.data.datasets[0].data.shift();
    }
    energyChart.update();

    if (isOverload === 1) {
        if (!isOverloadActive) { 
            // Kích hoạt hàm tương tác chuỗi khối thật
            sendToBlockchain(timeStr, wattValue);
            isOverloadActive = true; 
        }
    } else {
        isOverloadActive = false; 
    }
}

// Vòng lặp Real-time quét dữ liệu mỗi 1.5 giây
setInterval(()=>{
    fetch("/data").then(r=>r.json()).then(d=>{
        document.getElementById("watt").textContent = Number(d.watt).toFixed(1);
        
        let st = document.getElementById("status");
        if(d.overload === 1) {
            st.textContent = "QUÁ TẢI - ĐANG SA THẢI TẢI!";
            st.style.background = "#ef4444";
        } else {
            st.textContent = "HỆ THỐNG AN TOÀN";
            st.style.background = "#10b981";
        }

        let now = new Date();
        let timeStr = now.getHours().toString().padStart(2, '0') + ":" + 
                      now.getMinutes().toString().padStart(2, '0') + ":" + 
                      now.getSeconds().toString().padStart(2, '0');

        updateChartAndTable(timeStr, d.watt, d.overload);
    }).catch(()=>{});
}, 1500);
</script>
</body>
</html>
)ENDHTML";

#endif