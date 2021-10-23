const functions = require('./build/Release/crc_addon.node'); 

const row_1 = new Uint8Array([0x00, 0x00, 0x00, 0x00]);
const row_2 = new Uint8Array([0xF2, 0x01, 0x83]);
const row_3 = new Uint8Array([0x0F, 0xAA, 0x00, 0x55]);
const row_4 = new Uint8Array([0x00, 0xFF, 0x55, 0x11]);
const row_5 = new Uint8Array([0x33, 0x22, 0x55, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF]);
const row_6 = new Uint8Array([0x92, 0x6B, 0x55]);
const row_7 = new Uint8Array([0xFF, 0xFF, 0xFF, 0xFF]);

const check_str = '123456789';

test('crc8 check matches AUTOSAR table', () => {
  expect(functions.crc8(row_1)).toBe(0x59);
  expect(functions.crc8(row_2)).toBe(0x37);
  expect(functions.crc8(row_3)).toBe(0x79);
  expect(functions.crc8(row_4)).toBe(0xB8);
  expect(functions.crc8(row_5)).toBe(0xCB);
  expect(functions.crc8(row_6)).toBe(0x8C);
  expect(functions.crc8(row_7)).toBe(0x74);

  expect(functions.crc8(check_str)).toBe(0x4B);
});

test('crc8h2f check matches AUTOSAR table', () => {
  expect(functions.crc8_h2f(row_1)).toBe(0x12);
  expect(functions.crc8_h2f(row_2)).toBe(0xC2);
  expect(functions.crc8_h2f(row_3)).toBe(0xC6);
  expect(functions.crc8_h2f(row_4)).toBe(0x77);
  expect(functions.crc8_h2f(row_5)).toBe(0x11);
  expect(functions.crc8_h2f(row_6)).toBe(0x33);
  expect(functions.crc8_h2f(row_7)).toBe(0x6C);

  expect(functions.crc8_h2f(check_str)).toBe(0xDF);
});

test('crc16 check matches AUTOSAR table', () => {
  expect(functions.crc16(row_1)).toBe(0x84C0);
  expect(functions.crc16(row_2)).toBe(0xD374);
  expect(functions.crc16(row_3)).toBe(0x2023);
  expect(functions.crc16(row_4)).toBe(0xB8F9);
  expect(functions.crc16(row_5)).toBe(0xF53F);
  expect(functions.crc16(row_6)).toBe(0x0745);
  expect(functions.crc16(row_7)).toBe(0x1D0F);

  expect(functions.crc16(check_str)).toBe(0x29B1);
});

test('crc32 check matches AUTOSAR table', () => {
  expect(functions.crc32(row_1)).toBe(0x2144DF1C);
  expect(functions.crc32(row_2)).toBe(0x24AB9D77);
  expect(functions.crc32(row_3)).toBe(0xB6C9B287);
  expect(functions.crc32(row_4)).toBe(0x32A06212);
  expect(functions.crc32(row_5)).toBe(0xB0AE863D);
  expect(functions.crc32(row_6)).toBe(0x9CDEA29B);
  expect(functions.crc32(row_7)).toBe(0xFFFFFFFF);

  expect(functions.crc32(check_str)).toBe(0xCBF43926);
});

test('crc32_p4 check matches AUTOSAR table', () => {
  expect(functions.crc32_p4(row_1)).toBe(0x6FB32240);
  expect(functions.crc32_p4(row_2)).toBe(0x4F721A25);
  expect(functions.crc32_p4(row_3)).toBe(0x20662DF8);
  expect(functions.crc32_p4(row_4)).toBe(0x9BD7996E);
  expect(functions.crc32_p4(row_5)).toBe(0xA65A343D);
  expect(functions.crc32_p4(row_6)).toBe(0xEE688A78);
  expect(functions.crc32_p4(row_7)).toBe(0xFFFFFFFF);

  expect(functions.crc32_p4(check_str)).toBe(0x1697D06A);
});

test('crc64 check matches AUTOSAR table', () => {
  expect(functions.crc32_p4(row_1)).toBe(0x6FB32240);
  expect(functions.crc32_p4(row_2)).toBe(0x4F721A25);
  expect(functions.crc32_p4(row_3)).toBe(0x20662DF8);
  expect(functions.crc32_p4(row_4)).toBe(0x9BD7996E);
  expect(functions.crc32_p4(row_5)).toBe(0xA65A343D);
  expect(functions.crc32_p4(row_6)).toBe(0xEE688A78);
  expect(functions.crc32_p4(row_7)).toBe(0xFFFFFFFF);

  expect(functions.crc32_p4(check_str)).toBe(0x1697D06A);
});
