'use strict';
const assert = require('node:assert/strict');
const fs = require('node:fs');
const os = require('node:os');
const path = require('node:path');
const net = require('node:net');
const {spawn} = require('node:child_process');

async function freePort() {
  const socket = net.createServer();
  await new Promise(resolve => socket.listen(0, '127.0.0.1', resolve));
  const port = socket.address().port;
  await new Promise(resolve => socket.close(resolve));
  return port;
}

async function main() {
  const binary = process.argv[2];
  if (!binary) throw new Error('Chạy: node tests/web_smoke.js <đường-dẫn-hospital_web>');
  const directory = fs.mkdtempSync(path.join(os.tmpdir(), 'carequeue-smoke-'));
  const dataPath = path.join(directory, 'hospital-data.txt');
  const port = await freePort();
  const base = 'http://127.0.0.1:' + port;
  const auth = 'Basic ' + Buffer.from('admin:smoke-password').toString('base64');
  let server;
  async function start() {
    server = spawn(binary, [], {
      cwd: path.resolve(__dirname, '..'),
      env: {...process.env, HOST:'127.0.0.1', PORT:String(port),
            HOSPITAL_DATA:dataPath, ADMIN_PASSWORD:'smoke-password'},
      stdio:'ignore'
    });
    for (let i=0; i<50; i++) {
      try { if ((await fetch(base+'/health')).ok) return; }
      catch (_) {}
      await new Promise(resolve => setTimeout(resolve, 100));
    }
    throw new Error('Server không khởi động.');
  }
  async function stop() {
    if (!server || server.exitCode !== null) return;
    const child = server;
    await new Promise(resolve => { child.once('exit', resolve); child.kill(); });
  }
  async function get(route, protectedRoute=true) {
    return fetch(base+route, {headers:protectedRoute?{Authorization:auth}:{}});
  }
  async function post(route, fields) {
    const response = await fetch(base+route, {
      method:'POST',
      headers:{Authorization:auth,'Content-Type':'application/x-www-form-urlencoded'},
      body:new URLSearchParams(fields)
    });
    const json = await response.json();
    assert(response.ok && json.ok, JSON.stringify(json));
    return json;
  }
  async function rejectedPost(route, fields) {
    const response = await fetch(base+route, {
      method:'POST',
      headers:{Authorization:auth,'Content-Type':'application/x-www-form-urlencoded'},
      body:new URLSearchParams(fields)
    });
    assert.equal(response.status,400);
    return (await response.json()).message;
  }
  const register = (name, triage, department='Nhi') =>
    post('/api/register',{name,birthYear:2003,gender:'male',category:'service',
      symptom:'Kiểm tra',department,triage});
  try {
    await start();
    const stalled = net.connect(port,'127.0.0.1');
    await new Promise(resolve => stalled.once('connect',resolve));
    stalled.write('GET /health HTTP/1.1\r\nHost: localhost\r\n');
    assert.equal((await fetch(base+'/health',{signal:AbortSignal.timeout(8000)})).status,200);
    stalled.destroy();
    assert.equal((await fetch(base+'/api/state')).status,401);
    assert.equal((await fetch(base+'/')).status,401);
    assert.equal((await fetch(base+'/api/register',{method:'POST',headers:{Authorization:auth,Origin:'https://other.example','Content-Type':'application/x-www-form-urlencoded'},body:'name=x'})).status,400);
    assert.equal((await get('/')).status,200);
    assert.equal((await get('/waiting',false)).status,200);
    const demo=await (await get('/queue-demo',false)).text();
    assert(demo.includes('</html>'));
    const script=demo.match(/<script>([\s\S]*?)<\/script>/);
    assert(script,'Trang minh họa Queue thiếu đoạn JavaScript hoàn chỉnh.');
    new Function(script[1]);
    const a=(await register('TEST_PRIVATE_A',5)).ticket;
    const b=(await register('TEST_PRIVATE_B',5)).ticket;
    const c=(await register('TEST_PRIVATE_C',1)).ticket;
    let state=await (await get('/api/state')).json();
    assert.deepEqual(state.waiting.map(p=>p.ticket),[c,a,b]);
    assert.equal(state.waiting.find(p=>p.ticket===b).waitMinutes,20);
    const waiting=await (await get('/api/waiting-display',false)).text();
    assert(!waiting.includes('TEST_PRIVATE'));
    assert.equal((await post('/api/call-next',{department:'Nhi'})).ticket,c);
    state=await (await get('/api/state')).json();
    assert.equal(state.waiting.find(p=>p.ticket===a).waitMinutes,10);
    assert.equal(state.waiting.find(p=>p.ticket===b).waitMinutes,20);
    assert.match(await rejectedPost('/api/call-next',{department:'Nhi'}),/Phong dang kham/);
    assert.match(await rejectedPost('/api/call-specific',{ticket:b}),/Phong dang kham/);
    await post('/api/complete',{ticket:c});
    assert.equal((await post('/api/call-specific',{ticket:b})).ticket,b);
    await post('/api/absent',{ticket:b});
    await post('/api/transfer',{ticket:a});
    state=await (await get('/api/state')).json();
    assert.equal(state.waiting[0].department,'Cap cuu UTC2');
    assert.equal((await post('/api/call-next',{})).ticket,a);
    await post('/api/transfer-department',{ticket:a,department:'Mat'});
    state=await (await get('/api/state')).json();
    assert.equal(state.waiting[0].department,'Mat');
    await post('/api/cancel',{ticket:a});
    const d=(await register('TEST_PRIVATE_D',5)).ticket;
    const e=(await register('TEST_PRIVATE_E',1)).ticket;
    await stop();
    await start();
    state=await (await get('/api/state')).json();
    assert.deepEqual(state.waiting.map(p=>p.ticket),[e,d]);
    assert.deepEqual(state.patients.slice(0,3).map(p=>p.status),['Da huy','Vang mat','Da kham xong']);
    assert.equal(state.history.length,3);
    assert.equal((await post('/api/call-next',{department:'Nhi'})).ticket,e);
    const f=(await register('TEST_PRIVATE_F',5,'Mat')).ticket;
    assert.equal((await post('/api/call-next',{})).ticket,f);
    console.log('PASS: quyền truy cập, FIFO/ưu tiên, phòng bận, thời gian chờ, timeout, chuyển khoa, lưu dữ liệu.');
  } finally {
    await stop();
    for (const file of fs.readdirSync(directory)) fs.unlinkSync(path.join(directory,file));
    fs.rmdirSync(directory);
  }
}
main().catch(error=>{console.error(error);process.exitCode=1;});
