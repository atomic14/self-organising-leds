import React, { useState } from "react";
import RawCanvas from "./RawCanvas";
import * as api from "../api/ledsApi";
import * as imageProcessing from "../imageProcessing/imageProcessing";
import Switch from "./Switch";
import Camera from "./Camera";
import _ from 'lodash';

export default function LedsUserInterface() {
  const [currentFrame, setCurrentFrame] = useState<Uint8Array>();
  const [ledPositions, setLedPositions] = useState<api.ILedPosition[]>();
  const [usePhoneCamera, setUsePhoneCamera] = useState(false);
  const [audioScale, setAudioScale] = useState(20)
  const [video, setVideo] = useState<HTMLVideoElement>();

  const isDev = window.location.hostname === "localhost";

  const [espAddress, setEspAddress] = useState("");

  async function clear() {
    await api.clear(espAddress);
  }

  async function calibrate() {
    if (usePhoneCamera && video) {
      await imageProcessing.calibrateUsingCamera(espAddress, video);
    } else {
      await api.calibrate(espAddress);
    }
  }

  async function pause() {
    await api.pause(espAddress);
  }

  async function runAnimation() {
    await api.runAnimation(espAddress);
  }

  async function runAudio() {
    await api.runAudio(espAddress);
  }

  async function runFlames() {
    await api.runFlames(espAddress);
  }

  async function runRandom() {
    await api.runRandom(espAddress);
  }

  async function grabFrame() {
    const bytes = await api.getFrame(espAddress);
    setCurrentFrame(bytes);
  }

  async function showLedPositions() {
    const bytes = await api.getFrame(espAddress);
    const ledPositions = await api.getLedPositions(espAddress);
    setCurrentFrame(bytes);
    setLedPositions(ledPositions);
  }

  async function _updateAudioScale(scale: number) {
    console.log(scale / 20.0);
    setAudioScale(scale);
    api.setAudioScale(espAddress, scale / 20.0);
  }

  const updateAudioScale = _.throttle(_updateAudioScale, 100);

  return (
    <div style={{ width: 320, display: "flex", flexDirection: "column" }}>
      <Switch checked={usePhoneCamera} onChange={setUsePhoneCamera} />
      {usePhoneCamera ? (
        <Camera
          ledPositions={ledPositions}
          onVideoReady={(video) => setVideo(video)}
        />
      ) : (
        <RawCanvas
          imageBytes={currentFrame}
          ledPositions={ledPositions}
          width={160}
          height={120}
        />
      )}
      {isDev && (
        <div
          style={{ display: "flex", flexDirection: "column", marginTop: 20 }}
        >
          <label htmlFor="espCamAddressInput">
            Enter the address of the esp32 board
          </label>
          <input
            id="espCamAddressInput"
            type="text"
            value={espAddress}
            onChange={(e) => setEspAddress(e.target.value)}
          />
        </div>
      )}
      <button onClick={clear}>Clear</button>
      <button onClick={calibrate}>Calibrate</button>
      <button onClick={pause}>Pause</button>
      <button onClick={runAnimation}>Run Animation</button>
      <button onClick={runAudio}>Run Audio</button>
      <button onClick={runFlames}>Run Flames</button>
      <button onClick={runRandom}>Run Random</button>
      {!usePhoneCamera && (
        <>
          <button onClick={showLedPositions}>Show Locations</button>
          <button onClick={() => grabFrame()}>Grab Frame</button>
        </>
      )}
      <input value={audioScale} min={0} max={100} style={{ marginTop: 40, marginBottom: 40 }} type="range" onChange={(event: React.ChangeEvent<HTMLInputElement>) => { updateAudioScale(event.currentTarget.valueAsNumber) }}></input>
    </div>
  );
}
