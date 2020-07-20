import * as React from 'react';
import styled from 'styled-components';
import { colors } from 'components/foundations/variables';

type NavButtonProps = React.ButtonHTMLAttributes<HTMLButtonElement> & {
  height?: number;
  width?: number;
  fill?: string;
  icon?: 'hamburger' | 'x';
};

const Root = styled('button')`
  display: inline-block;
  margin: 0;
  padding: 0;
  height: 100%;
  background: none;
  border: none;
  color: inherit;
  cursor: pointer;

  &:hover,
  &:focus {
    outline: none;
  }
`;

const VisuallyHidden = styled('span')`
  border: 0;
  clip: rect(0 0 0 0);
  height: 1px;
  width: 1px;
  margin: -1px;
  padding: 0;
  overflow: hidden;
  position: absolute;
`;

const NavButton: React.FC<NavButtonProps> = ({ height, width, fill, icon, children, ...rest }) => {
  if (icon === 'hamburger') {
    return (
      <Root {...rest}>
        <VisuallyHidden>{children}</VisuallyHidden>
        <svg width={width} height={height} viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg">
          <g clipPath="url(#clip0)">
            <path d="M21 7H3C2.4 7 2 6.6 2 6C2 5.4 2.4 5 3 5H21C21.6 5 22 5.4 22 6C22 6.6 21.6 7 21 7Z" fill={fill} />
            <path
              d="M21 13H3C2.4 13 2 12.6 2 12C2 11.4 2.4 11 3 11H21C21.6 11 22 11.4 22 12C22 12.6 21.6 13 21 13Z"
              fill={fill}
            />
            <path
              d="M21 19H3C2.4 19 2 18.6 2 18C2 17.4 2.4 17 3 17H21C21.6 17 22 17.4 22 18C22 18.6 21.6 19 21 19Z"
              fill={fill}
            />
          </g>
          <defs>
            <clipPath id="clip0">
              <rect width="24" height="24" fill="white" />
            </clipPath>
          </defs>
        </svg>
      </Root>
    );
  }

  return (
    <Root {...rest}>
      <VisuallyHidden>{children}</VisuallyHidden>
      <svg width={width} height={height} viewBox="0 0 16 16" fill="none" xmlns="http://www.w3.org/2000/svg">
        <path
          fill={fill}
          d="M4.47329 3.52666C4.34776 3.40112 4.1775 3.3306 3.99996 3.3306C3.82243 3.3306 3.65216 3.40112 3.52663 3.52666C3.40109 3.65219 3.33057 3.82246 3.33057 3.99999C3.33057 4.17753 3.40109 4.34779 3.52663 4.47332L7.05996 7.99999L7.99996 7.05999L4.47329 3.52666Z"
        />
        <path
          fill={fill}
          d="M11.5267 12.4733C11.5886 12.5358 11.6624 12.5854 11.7436 12.6193C11.8249 12.6531 11.912 12.6705 12 12.6705C12.088 12.6705 12.1751 12.6531 12.2564 12.6193C12.3376 12.5854 12.4114 12.5358 12.4733 12.4733C12.5358 12.4114 12.5854 12.3376 12.6193 12.2564C12.6531 12.1751 12.6705 12.088 12.6705 12C12.6705 11.912 12.6531 11.8249 12.6193 11.7436C12.5854 11.6624 12.5358 11.5886 12.4733 11.5267L8.94 8L8 8.94L11.5267 12.4733Z"
        />
        <path
          fill={fill}
          d="M3.52667 12.4733C3.58864 12.5358 3.66238 12.5854 3.74362 12.6193C3.82485 12.6531 3.91199 12.6705 4 12.6705C4.08801 12.6705 4.17515 12.6531 4.25638 12.6193C4.33762 12.5854 4.41136 12.5358 4.47333 12.4733L8 8.93999L8.94 7.99999L12.4733 4.47332C12.5989 4.34779 12.6694 4.17753 12.6694 3.99999C12.6694 3.82246 12.5989 3.65219 12.4733 3.52666C12.3478 3.40112 12.1775 3.3306 12 3.3306C11.8225 3.3306 11.6522 3.40112 11.5267 3.52666L8 7.05999L7.06 7.99999L3.52667 11.5267C3.46418 11.5886 3.41459 11.6624 3.38074 11.7436C3.34689 11.8248 3.32947 11.912 3.32947 12C3.32947 12.088 3.34689 12.1751 3.38074 12.2564C3.41459 12.3376 3.46418 12.4113 3.52667 12.4733Z"
        />
      </svg>
    </Root>
  );
};

NavButton.defaultProps = {
  height: 24,
  width: 24,
  fill: colors.grey05,
  icon: 'hamburger'
};

export default NavButton;
