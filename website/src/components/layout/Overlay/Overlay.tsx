import * as React from 'react';
import styled, { css } from 'styled-components';

import { NavigationContext } from '../Navigation/NavigationContext';
import { breakpoints, layerIndexes } from 'components/foundations/variables';

interface OverlayProps {
  visible?: boolean;
}

const Visible = css`
  @media (max-width: ${breakpoints.lg - 1}px) {
    opacity: 1;
    visibility: visible;
  }
`;

const Root = styled('div')<OverlayProps>`
  position: fixed;
  top: 0;
  left: 0;
  bottom: 0;
  right: 0;
  background: rgba(0, 31, 63, 0.6);
  z-index: ${layerIndexes.overlay};
  opacity: 0;
  visibility: hidden;
  transition: all 0.3s ease;

  ${props => props.visible && Visible}
`;

const Overlay: React.FC = () => {
  const { state } = React.useContext(NavigationContext);

  return <Root visible={state.isOpen} />;
};

export default Overlay;
