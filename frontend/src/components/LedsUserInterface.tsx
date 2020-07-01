import React, { useState } from "react";
import RawCanvas from "./RawCanvas";
import * as api from "../api/ledsApi";

export default function LedsUserInterface() {
  const [currentFrame, setCurrentFrame] = useState<Uint8Array>();
  const [ledPositions, setLedPositions] = useState<api.ILedPosition[]>();

  const isDev = window.location.hostname === "localhost";

  const [espCamAddress, setEspCamAddress] = useState(
    isDev ? "http://espcam.local" : ""
  );

  async function clear() {
    await api.clear(espCamAddress);
  }

  async function calibrate() {
    await api.calibrate(espCamAddress);
  }

  async function pause() {
    await api.pause(espCamAddress);
  }

  async function run() {
    await api.run(espCamAddress);
  }

  async function grabFrame() {
    const bytes = await api.getFrame(espCamAddress);
    setCurrentFrame(bytes);
  }

  async function showLedPositions() {
    const bytes = await api.getFrame(espCamAddress);
    const ledPositions = await api.getLedPositions(espCamAddress);
    setCurrentFrame(bytes);
    setLedPositions(ledPositions);
  }

  return (
    <div style={{ width: 320, display: "flex", flexDirection: "column" }}>
      <RawCanvas
        imageBytes={currentFrame}
        ledPositions={ledPositions}
        width={160}
        height={120}
      />
      {isDev && (
        <div
          style={{ display: "flex", flexDirection: "column", marginTop: 20 }}
        >
          <label htmlFor="espCamAddressInput">
            Enter the address of the esp cam
          </label>
          <input
            id="espCamAddressInput"
            type="text"
            value={espCamAddress}
            onChange={(e) => setEspCamAddress(e.target.value)}
          />
        </div>
      )}
      <button onClick={clear}>Clear</button>
      <button onClick={calibrate}>Calibrate</button>
      <button onClick={pause}>Pause</button>
      <button onClick={run}>Run</button>
      <button onClick={showLedPositions}>Show Locations</button>
      <button onClick={() => grabFrame()}>Grab Frame</button>
    </div>
  );
}
