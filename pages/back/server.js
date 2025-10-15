const express = require('express');
const path = require('path');
const app = express();
const port = 80;

// 中间件
app.use(express.json());
app.use(express.static(path.join(__dirname, '..')));

// 模拟当前颜色数据
let currentColor = { r: 255, g: 0, b: 0 }; // 默认红色

// CORS 处理
app.use((req, res, next) => {
  res.header('Access-Control-Allow-Origin', '*');
  res.header('Access-Control-Allow-Methods', 'GET, POST, PUT, DELETE, OPTIONS');
  res.header('Access-Control-Allow-Headers', 'Origin, X-Requested-With, Content-Type, Accept');
  next();
});

// GET /current-color - 返回当前颜色 (文本格式 "r,g,b")
app.get('/current-color', (req, res) => {
  console.log('GET /current-color requested');
  console.log('Returning current color:', currentColor);
  res.send(`${currentColor.r},${currentColor.g},${currentColor.b}`);
});

// POST /set-color - 设置颜色 (支持URL参数)
app.post('/set-color', (req, res) => {
  console.log('POST /set-color requested with query:', req.query);
  
  // 从URL参数获取颜色值
  let { r, g, b } = req.query;
  
  // 转换为数字
  r = parseInt(r);
  g = parseInt(g);
  b = parseInt(b);
  
  // 验证颜色值
  if (
    isNaN(r) || isNaN(g) || isNaN(b) ||
    r < 0 || r > 255 ||
    g < 0 || g > 255 ||
    b < 0 || b > 255
  ) {
    console.log('Invalid color values received:', req.query);
    return res.status(400).json({ error: 'Invalid color values. RGB values must be numbers between 0-255.' });
  }
  
  // 更新颜色
  currentColor = { r, g, b };
  console.log('Color updated to:', currentColor);
  
  // 返回成功响应
  res.json({ 
    success: true, 
    message: `Color updated to RGB(${r}, ${g}, ${b})` 
  });
});

// 提供index.html文件
app.get('/', (req, res) => {
  console.log('GET / requested, serving index.html');
  res.sendFile(path.join(__dirname, '../before/index.html'));
});

// 启动服务器
app.listen(port, () => {
  console.log(`Magic-Mini mock server is running at http://localhost:${port}`);
  console.log(`Current color is set to RGB(${currentColor.r}, ${currentColor.g}, ${currentColor.b})`);
});