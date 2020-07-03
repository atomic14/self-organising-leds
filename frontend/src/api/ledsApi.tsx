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

function blobToArrayBuffer(blob: Blob): Promise<ArrayBuffer> {
  return new Promise((resolve, reject) => {
    let fr = new FileReader();
    fr.onload = () => {
      if (fr.result) {
        resolve(fr.result as ArrayBuffer);
      } else {
        reject(new Error("Failed to load"));
      }
    };
    fr.readAsArrayBuffer(blob);
  });
}

export async function getLedPositions(host: string): Promise<ILedPosition[]> {
  const response = await fetch(`${host}/led_positions`);
  const blob = await response.blob();

  // const arrayBuffer = await (blob as any).arrayBuffer();
  const arrayBuffer = await blobToArrayBuffer(blob);
  const uint16Array = new Uint16Array(arrayBuffer);
  const results = [];
  for (let i = 0; i < uint16Array.length / 2; i++) {
    results.push({ x: uint16Array[i * 2], y: uint16Array[i * 2 + 1] });
  }
  return results;
}

export async function setLedPositions(
  host: string,
  ledPositions: ILedPosition[]
) {
  const payload = new Uint16Array(ledPositions.length * 2);
  for (let i = 0; i < ledPositions.length; i++) {
    payload[i * 2] = ledPositions[i].x;
    payload[i * 2 + 1] = ledPositions[i].y;
  }
  await fetch(`${host}/positions`, {
    method: "POST",
    body: payload,
  });
}

export interface ILedColor {
  r: number;
  g: number;
  b: number;
}

export async function setLeds(host: string, colors: ILedColor[]) {
  const payload = new Uint8Array(colors.length * 3);
  console.log("Payload is", payload.length);
  colors.forEach((color, index) => {
    payload[index * 3] = color.r;
    payload[index * 3 + 1] = color.g;
    payload[index * 3 + 2] = color.b;
  });
  await fetch(`${host}/leds`, {
    method: "POST",
    body: payload,
  });
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
