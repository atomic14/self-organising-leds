// get a raw frame from the camera - 160x120 pixels greyscale
export async function getFrame(host: string): Promise<Uint8Array> {
  const response = await fetch(`${host}/raw_image`);
  const blob = await response.blob();
  const arrayBuffer = await (blob as any).arrayBuffer();
  const uint8Array = new Uint8Array(arrayBuffer);
  return uint8Array;
}

// clear all the leds
export async function clear(host: string) {
  await fetch(`${host}/clear`);
}

export interface ILedPosition {
  x: number;
  y: number;
}

// get the locations of the leds
export async function getLedPositions(host: string): Promise<ILedPosition[]> {
  const response = await fetch(`${host}/led_positions`);
  const blob = await response.blob();
  const arrayBuffer = await (blob as any).arrayBuffer();
  const uint16Array = new Uint16Array(arrayBuffer);
  const results = [];
  for (let i = 0; i < uint16Array.length / 2; i++) {
    results.push({ x: uint16Array[i * 2], y: uint16Array[i * 2 + 1] });
  }
  return results;
}

// pause the animation loop
export async function pause(host: string) {
  await fetch(`${host}/pause`);
}

// resume the animation loop
export async function run(host: string) {
  await fetch(`${host}/run`);
}

// start the calibration process
export async function calibrate(host: string) {
  await fetch(`${host}/calibrate`);
}
