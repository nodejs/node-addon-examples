import React from 'react';
import styled from 'styled-components';
import { colors, layerIndexes, breakpoints } from 'components/foundations/variables';

interface ToggleableProps {
  isOpen?: boolean;
}

const Wrapper = styled('button')<ToggleableProps>`
  display: inline-block;
  position: fixed;
  bottom: 24px;
  right: 24px;
  padding: 0;
  background-color: ${props => (props.isOpen ? colors.red06 : colors.blue06)};
  color: ${colors.white};
  cursor: pointer;
  z-index: ${layerIndexes.overlay - 1};
  border-radius: 50%;
  border: 1px solid rgba(255, 255, 255, 0.1);
  box-shadow: 0 2px 4px 1px rgba(0, 0, 0, 0.15);

  &:hover,
  &:focus {
    outline: none;
  }

  @media (min-width: ${breakpoints.lg}px) and (max-width: ${breakpoints.xl - 1}px) {
    z-index: ${layerIndexes.dialog + 2};
  }

  @media (min-width: ${breakpoints.xl}px) {
    display: none;
  }
`;

const Inner = styled('div')<ToggleableProps>`
  display: flex;
  flex-direction: row;
  align-items: center;
  justify-content: center;
  width: 60px;
  height: 60px;

  &.is-open {
    & svg {
      transform: rotate(180deg);
    }
  }
`;

interface TocFloatingButtonProps {
  className?: string;
  tocIsOpen?: boolean;
  onClick?: (e?: React.MouseEvent<HTMLButtonElement>) => void;
}

const TocFloatingButton: React.SFC<TocFloatingButtonProps> = ({ className, tocIsOpen, onClick }) => (
  <Wrapper className={className} onClick={onClick} isOpen={tocIsOpen} aria-label="Toggle Table of Contents">
    <Inner isOpen={tocIsOpen} className={tocIsOpen ? 'is-open' : undefined}>
      <svg width="24" height="24" viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg">
        <path
          d="M4.71 16.71L12 9.41L19.29 16.71C19.383 16.8037 19.4936 16.8781 19.6154 16.9289C19.7373 16.9797 19.868 17.0058 20 17.0058C20.132 17.0058 20.2627 16.9797 20.3846 16.9289C20.5064 16.8781 20.617 16.8037 20.71 16.71C20.8037 16.617 20.8781 16.5064 20.9289 16.3846C20.9797 16.2627 21.0058 16.132 21.0058 16C21.0058 15.868 20.9797 15.7373 20.9289 15.6154C20.8781 15.4936 20.8037 15.383 20.71 15.29L12.71 7.29C12.617 7.19627 12.5064 7.12188 12.3846 7.07111C12.2627 7.02034 12.132 6.9942 12 6.9942C11.868 6.9942 11.7373 7.02034 11.6154 7.07111C11.4936 7.12188 11.383 7.19627 11.29 7.29L3.29 15.29C3.1017 15.4783 2.99591 15.7337 2.99591 16C2.99591 16.2663 3.1017 16.5217 3.29 16.71C3.47831 16.8983 3.7337 17.0041 4 17.0041C4.2663 17.0041 4.5217 16.8983 4.71 16.71Z"
          fill={colors.white}
        />
      </svg>
    </Inner>
  </Wrapper>
);

export default TocFloatingButton;
