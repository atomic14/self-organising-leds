import React, { useRef, useEffect, useState } from "react";
import { ILedPosition } from "../api/ledsApi";

export default function Camera({
  onVideoReady,
}: {
  ledPositions: ILedPosition[] | undefined;
  onVideoReady: (video: HTMLVideoElement) => void;
}) {
  const videoRef = useRef<HTMLVideoElement>(null);
  const [videoWidth, setVideoWidth] = useState(100);
  const [videoHeight, setVideoHeight] = useState(100);
  // wire up the video stream
  useEffect(() => {
    const video = videoRef.current;
    async function startVideo() {
      if (video) {
        const stream = await navigator.mediaDevices.getUserMedia({
          video: { facingMode: "environment" },
          audio: false,
        });
        const canPlayListener = () => {
          onVideoReady(video);
          alert(`canPlay 1 ${video.videoWidth} x ${video.videoHeight}`);
          setVideoWidth(video.videoWidth);
          setVideoHeight(video.videoHeight);
          video.removeEventListener("canplay", canPlayListener);
        };
        video.addEventListener("canplay", canPlayListener);
        video.srcObject = stream;
        video.play();
      }
    }
    startVideo();
  }, [videoRef, onVideoReady]);

  const scale = 320 / videoWidth;

  return (
    <video
      ref={videoRef}
      width={videoWidth * scale}
      height={videoHeight * scale}
      style={{ width: videoWidth * scale, height: videoHeight * scale }}
      muted
      playsInline
    />
  );
}
