import * as api from "../api/ledsApi";

function getVideoFrame(video: HTMLVideoElement, canvas: HTMLCanvasElement) {
  const width = video.videoWidth;
  const height = video.videoHeight;
  const context = canvas.getContext("2d");
  // draw the video to the canvas
  context!.drawImage(video, 0, 0, width, height);
  // get the raw image bytes
  const imageData = context!.getImageData(0, 0, width, height);
  // convert to greyscale
  const bytes = new Uint8Array(width * height);
  for (let y = 0; y < height; y++) {
    for (let x = 0; x < width; x++) {
      const r = imageData.data[(y * width + x) * 4];
      const g = imageData.data[(y * width + x) * 4 + 1];
      const b = imageData.data[(y * width + x) * 4 + 2];
      // https://en.wikipedia.org/wiki/Grayscale#Converting_color_to_grayscale
      const grey = Math.min(255, 0.299 * r + 0.587 * g + 0.114 * b);
      bytes[y * width + x] = grey;
    }
  }
  return bytes;
}

function blur(image: Uint8Array, width: number, height: number): Uint8Array {
  const kernel = [
    0.03426,
    0.037671,
    0.04101,
    0.044202,
    0.047168,
    0.049832,
    0.052124,
    0.053979,
    0.055344,
    0.05618,
    0.056461,
    0.05618,
    0.055344,
    0.053979,
    0.052124,
    0.049832,
    0.047168,
    0.044202,
    0.04101,
    0.037671,
    0.03426,
  ];

  const horizontal = new Uint8Array(width * height);
  for (let y = 0; y < height; y++) {
    const row = y * width;
    for (let x = 0; x < width; x++) {
      let value = 0;
      let divider = 0;
      for (let k = -10; k <= 10; k++) {
        if (k + x >= 0 && k + x < width) {
          value += image[row + x + k] * kernel[k + 10];
          divider += kernel[k + 10];
        }
      }
      horizontal[row + x] = value / divider;
    }
  }
  const vertical = new Uint8Array(width * height);
  for (let y = 0; y < height; y++) {
    for (let x = 0; x < width; x++) {
      let value = 0;
      let divider = 0;
      for (let k = -10; k <= 10; k++) {
        if (k + y >= 0 && k + y < height) {
          value += horizontal[(k + y) * width + x] * kernel[k + 10];
          divider += kernel[k + 10];
        }
      }
      vertical[y * width + x] = value / divider;
    }
  }
  return vertical;
}

function debugImage(bytes: Uint8Array, canvas: HTMLCanvasElement) {
  const width = canvas.width;
  const height = canvas.height;
  const context = canvas.getContext("2d");
  const imageData = context!.getImageData(0, 0, width, height);
  for (let y = 0; y < height; y++) {
    for (let x = 0; x < width; x++) {
      imageData.data[(y * width + x) * 4] = bytes[y * width + x];
      imageData.data[(y * width + x) * 4 + 1] = bytes[y * width + x];
      imageData.data[(y * width + x) * 4 + 2] = bytes[y * width + x];
    }
  }
  context!.putImageData(imageData, 0, 0);
  return bytes;
}

export async function calibrateUsingCamera(
  espHost: string,
  video: HTMLVideoElement
) {
  const width = video.videoWidth;
  const height = video.videoHeight;
  // create a canvas element to capture video stills
  const canvas = document.createElement("canvas");
  canvas.width = width;
  canvas.height = height;
  // get the current led position - this will tell us how many LEDs there are
  const ledPositions = await api.getLedPositions(espHost);
  // loop through each LED
  const colors = new Array<api.ILedColor>(ledPositions.length);
  colors.fill({ r: 0, g: 0, b: 0 }, 0, colors.length);
  console.log(colors);
  // pause animations
  await api.pause(espHost);
  // clear all the leds
  await api.setLeds(espHost, colors);
  for (let ledIdx = 0; ledIdx < ledPositions.length; ledIdx++) {
    // await delay(200);
    console.log("Processing led ", ledIdx);
    // capture a frame with all the LEDs off
    const imageBytesNoLeds = blur(getVideoFrame(video, canvas), width, height);
    debugImage(imageBytesNoLeds, canvas);
    // light the current LED
    colors[ledIdx] = {
      r: 255,
      g: 255,
      b: 255,
    };
    await api.setLeds(espHost, colors);
    // await delay(200);
    // capture a frame
    const imageBytesOneLed = blur(getVideoFrame(video, canvas), width, height);
    debugImage(imageBytesOneLed, canvas);
    // get the maximum difference between the two images
    let maxDifference = 0;
    for (let i = 0; i < imageBytesNoLeds.length; i++) {
      maxDifference = Math.max(
        maxDifference,
        imageBytesOneLed[i] - imageBytesNoLeds[i]
      );
    }
    // now work out the approximate location of the led
    let xPos = 0;
    let yPos = 0;
    let total = 0;
    for (let y = 0; y < height; y++) {
      for (let x = 0; x < width; x++) {
        const index = y * width + x;
        const diff = imageBytesOneLed[index] - imageBytesNoLeds[index];
        if (diff > 0.5 * maxDifference) {
          xPos += x * diff;
          yPos += y * diff;
          total += diff;
        }
      }
    }
    ledPositions[ledIdx].x = xPos / total;
    ledPositions[ledIdx].y = yPos / total;
    // clear the current LED
    colors[ledIdx] = {
      r: 0,
      g: 0,
      b: 0,
    };
    await api.setLeds(espHost, colors);
  }
  const context = canvas.getContext("2d");
  context!.strokeStyle = "red";
  ledPositions.forEach((position) => {
    context!.strokeRect(position.x - 2, position.y - 2, 4, 4);
  });
  // send the LED positions up to the esp board
  await api.setLedPositions(espHost, ledPositions);
  // resume animations
  await api.runAnimation(espHost);
}
