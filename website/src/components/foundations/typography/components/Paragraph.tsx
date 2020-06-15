import * as React from 'react';
import styled from 'styled-components';

import { TextSizes } from '../../Theme';
import { determineFontDimensions } from '../utils';
import { Typography, TypographyProps } from './Typography';

/**
 * This is a base `Text` element to handle typography elements.
 */
const StyledText = styled(Typography)`
  letter-spacing: -0.05px;
`;

export interface ParagraphProps extends TypographyProps {
  /** Additional CSS classes to add to the component. */
  className?: string;
  /** Additional CSS properties to add to the component. */
  style?: React.CSSProperties;
  /** What HTML element to render the text as. */
  as?: keyof JSX.IntrinsicElements | React.ComponentType<any>;
  /** Size value of the text. */
  scale?: keyof TextSizes;
}

/**
 * Paragraph component provided as a styled component primitive.
 */
export const Paragraph: React.SFC<ParagraphProps> = ({ children, as, scale, ...rest }) => (
  <StyledText as={as} {...determineFontDimensions('paragraph', scale)} {...rest}>
    {children}
  </StyledText>
);

Paragraph.defaultProps = {
  as: 'p',
  color: 'grey07',
  scale: 300
};

Paragraph.displayName = 'Paragraph';
