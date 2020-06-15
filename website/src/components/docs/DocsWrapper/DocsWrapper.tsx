import { breakpoints } from 'components/foundations/variables';
import styled from 'styled-components';

interface DocsWrapperProps {
  hasToc?: boolean;
}

const DocsWrapper = styled('article')<DocsWrapperProps>`
  display: flex;
  flex-direction: column;
  flex: 1 1 auto;
  position: relative;
  padding: 32px;

  @media (min-width: ${breakpoints.lg}px) {
    flex-direction: ${props => props.hasToc && 'row-reverse'};
  }

  @media (max-width: ${breakpoints.lg - 1}px) {
    overflow-x: auto;
  }
`;

export default DocsWrapper;
