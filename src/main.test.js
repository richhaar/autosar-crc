const autocrc = require('./index.js'); 

const row_1 = new Uint8Array([0x00, 0x00, 0x00, 0x00]);
const row_2 = new Uint8Array([0xF2, 0x01, 0x83]);
const row_3 = new Uint8Array([0x0F, 0xAA, 0x00, 0x55]);
const row_4 = new Uint8Array([0x00, 0xFF, 0x55, 0x11]);
const row_5 = new Uint8Array([0x33, 0x22, 0x55, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF]);
const row_6 = new Uint8Array([0x92, 0x6B, 0x55]);
const row_7 = new Uint8Array([0xFF, 0xFF, 0xFF, 0xFF]);

const check_str = '123456789';

test('crc8 check matches AUTOSAR table', () => {
  expect(autocrc.crc8(row_1)).toBe(0x59);
  expect(autocrc.crc8(row_2)).toBe(0x37);
  expect(autocrc.crc8(row_3)).toBe(0x79);
  expect(autocrc.crc8(row_4)).toBe(0xB8);
  expect(autocrc.crc8(row_5)).toBe(0xCB);
  expect(autocrc.crc8(row_6)).toBe(0x8C);
  expect(autocrc.crc8(row_7)).toBe(0x74);

  expect(autocrc.crc8(check_str)).toBe(0x4B);
});

test('crc8h2f check matches AUTOSAR table', () => {
  expect(autocrc.crc8_h2f(row_1)).toBe(0x12);
  expect(autocrc.crc8_h2f(row_2)).toBe(0xC2);
  expect(autocrc.crc8_h2f(row_3)).toBe(0xC6);
  expect(autocrc.crc8_h2f(row_4)).toBe(0x77);
  expect(autocrc.crc8_h2f(row_5)).toBe(0x11);
  expect(autocrc.crc8_h2f(row_6)).toBe(0x33);
  expect(autocrc.crc8_h2f(row_7)).toBe(0x6C);

  expect(autocrc.crc8_h2f(check_str)).toBe(0xDF);
});

test('crc16 check matches AUTOSAR table', () => {
  expect(autocrc.crc16(row_1)).toBe(0x84C0);
  expect(autocrc.crc16(row_2)).toBe(0xD374);
  expect(autocrc.crc16(row_3)).toBe(0x2023);
  expect(autocrc.crc16(row_4)).toBe(0xB8F9);
  expect(autocrc.crc16(row_5)).toBe(0xF53F);
  expect(autocrc.crc16(row_6)).toBe(0x0745);
  expect(autocrc.crc16(row_7)).toBe(0x1D0F);

  expect(autocrc.crc16(check_str)).toBe(0x29B1);
});

test('crc32 check matches AUTOSAR table', () => {
  expect(autocrc.crc32(row_1)).toBe(0x2144DF1C);
  expect(autocrc.crc32(row_2)).toBe(0x24AB9D77);
  expect(autocrc.crc32(row_3)).toBe(0xB6C9B287);
  expect(autocrc.crc32(row_4)).toBe(0x32A06212);
  expect(autocrc.crc32(row_5)).toBe(0xB0AE863D);
  expect(autocrc.crc32(row_6)).toBe(0x9CDEA29B);
  expect(autocrc.crc32(row_7)).toBe(0xFFFFFFFF);

  expect(autocrc.crc32(check_str)).toBe(0xCBF43926);
});

test('crc32_p4 check matches AUTOSAR table', () => {
  expect(autocrc.crc32_p4(row_1)).toBe(0x6FB32240);
  expect(autocrc.crc32_p4(row_2)).toBe(0x4F721A25);
  expect(autocrc.crc32_p4(row_3)).toBe(0x20662DF8);
  expect(autocrc.crc32_p4(row_4)).toBe(0x9BD7996E);
  expect(autocrc.crc32_p4(row_5)).toBe(0xA65A343D);
  expect(autocrc.crc32_p4(row_6)).toBe(0xEE688A78);
  expect(autocrc.crc32_p4(row_7)).toBe(0xFFFFFFFF);

  expect(autocrc.crc32_p4(check_str)).toBe(0x1697D06A);
});

test('crc64 check matches AUTOSAR table', () => {
  expect(autocrc.crc64(row_1)).toBe(0xF4A586351E1B9F4B);
  expect(autocrc.crc64(row_2)).toBe(0x319C27668164F1C6);
  expect(autocrc.crc64(row_3)).toBe(0x54C5D0F7667C1575);
  expect(autocrc.crc64(row_4)).toBe(0xA63822BE7E0704E6);
  expect(autocrc.crc64(row_5)).toBe(0x701ECEB219A8E5D5);
  expect(autocrc.crc64(row_6)).toBe(0x5FAA96A9B59F3E4E);
  expect(autocrc.crc64(row_7)).toBe(0xFFFFFFFF00000000);

  expect(autocrc.crc64(check_str)).toBe(0x995DC9BBDF1939FA);
});

test('crc accepts string', () => {
	expect(autocrc.crc64('123456789')).toBe(0x995DC9BBDF1939FA);
});

test('crc accepts ArrayBuffer', () => {
	const buf = new Uint8Array([0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39]).buffer;
	expect(autocrc.crc64(buf)).toBe(0x995DC9BBDF1939FA);
});

test('crc accepts TypedArray', () => {
	const arr = new Uint8Array([0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39]);
	expect(autocrc.crc64(arr)).toBe(0x995DC9BBDF1939FA);
});
