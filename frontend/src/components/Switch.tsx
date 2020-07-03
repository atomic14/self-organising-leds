import React from "react";
import styled from "styled-components";

export default function Switch({
  checked,
  onChange,
}: {
  checked: boolean;
  onChange: (newValue: boolean) => void;
}) {
  return (
    <SwitchWrapper>
      <SwitchLabel>Use Phone Camera</SwitchLabel>
      <label className="switch">
        <input
          type="checkbox"
          checked={checked}
          onChange={(event) => onChange(event.target.checked)}
        />
        <span className="slider round"></span>
      </label>
    </SwitchWrapper>
  );
}

const SwitchWrapper = styled.div`
  display: flex;
  align-items: center;
`;

const SwitchLabel = styled.div`
  margin-right: 10px;
`;
