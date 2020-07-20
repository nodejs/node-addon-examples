import { createGlobalStyle } from 'styled-components';
import reboot from '../styles/reboot';
import base from '../styles/base';
import code from '../styles/code';

const GlobalStyles = createGlobalStyle`
${reboot}
${base}
${code}
`;

export default GlobalStyles;
