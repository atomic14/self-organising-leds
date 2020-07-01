import React, { useState, useEffect } from "react";
import "./App.css";

import LedsUserInterface from "./components/LedsUserInterface";

function App() {
  const [screenHeight, setScreenHeight] = useState(window.innerHeight);

  useEffect(() => {
    function onResize() {
      console.log("Screen size changed");
      setScreenHeight(window.innerHeight);
    }
    window.onresize = onResize;
    return function cancel() {
      window.onresize = null;
    };
  });

  return (
    <div className="site-content" style={{ height: screenHeight }}>
      <LedsUserInterface />
    </div>
  );
}

export default App;
