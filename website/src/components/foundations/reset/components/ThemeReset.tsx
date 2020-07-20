import * as React from 'react';

import { Theme } from '../../Theme';
import GlobalStyles from './GlobalStyles';

interface ResetProps {
  className?: string;
  style?: React.CSSProperties;
}

const ThemeReset: React.FC<ResetProps> = ({ children }) => {
  return (
    <Theme>
      <GlobalStyles />
      {children}
    </Theme>
  );
};

export default ThemeReset;
