import React, { useRef, useEffect } from "react";
import { ILedPosition } from "../api/ledsApi";

function RawCanvas({
  imageBytes,
  ledPositions,
  width,
  height,
}: {
  imageBytes: Uint8Array | undefined;
  ledPositions: ILedPosition[] | undefined;
  width: number;
  height: number;
}) {
  const canvasRef = useRef<HTMLCanvasElement>(null);

  useEffect(() => {
    const canvasElement = canvasRef.current;
    if (canvasElement) {
      const context = canvasElement.getContext("2d");
      if (!context) {
        console.warn("No context");
        return;
      }
      // draw the image
      if (imageBytes) {
        const imageData = context.createImageData(width, height);
        for (let y = 0; y < height; y++) {
          for (let x = 0; x < width; x++) {
            imageData.data[(x + y * width) * 4] = imageBytes[x + y * width];
            imageData.data[(x + y * width) * 4 + 1] = imageBytes[x + y * width];
            imageData.data[(x + y * width) * 4 + 2] = imageBytes[x + y * width];
            imageData.data[(x + y * width) * 4 + 3] = 255;
          }
        }
        context.putImageData(imageData, 0, 0);
      }
      // the the led positions (if they've been provided)
      if (ledPositions) {
        ledPositions.forEach((position) => {
          context.strokeStyle = "red";
          context.lineWidth = 1;
          context.strokeRect(position.x - 1, position.y - 1, 2, 2);
        });
      }
    }
  }, [canvasRef, imageBytes, ledPositions, width, height]);

  return (
    <canvas
      ref={canvasRef}
      width={width}
      height={height}
      style={{ width: 320, height: 240 }}
    />
  );
}

export default RawCanvas;
