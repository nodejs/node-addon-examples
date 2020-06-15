import React from 'react';
import styled, { css } from 'styled-components';
import { Link } from 'gatsby';

import { MenuNode, Edge, HeaderMenuItem } from 'interfaces/nodes';
import { determineFontDimensions, Heading } from 'components/foundations';
import { colors, layerIndexes, breakpoints, dimensions } from 'components/foundations/variables';
import { isActive } from 'utils/helpers';

import { NavigationContext, NavigationActionTypes } from './NavigationContext';
import NavigationMenu from './NavigationMenu';
import NavButton from './NavButton';

interface ToggleableProps {
  isOpen?: boolean;
}

const Wrapper = styled('aside')<ToggleableProps>`
  position: fixed;
  transition: all 0.3s ease;
  background-color: ${colors.white};
  box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
  z-index: ${layerIndexes.dialog};
  overflow-y: auto;

  @media (min-width: ${breakpoints.md}px) and (max-width: ${breakpoints.lg - 1}px) {
    width: ${dimensions.widths.sidebar.sm}px;
    box-shadow: none;
    border-bottom: none;
  }

  @media (max-width: ${breakpoints.lg - 1}px) {
    position: fixed;
    top: 0px;
    left: 0px;
    bottom: 0px;
    right: 0px;
    width: ${dimensions.widths.sidebar.md}px;
    height: 100vh;
    padding-bottom: 5rem;
    overflow-y: auto;
    pointer-events: auto;
    transform: translate(${props => (props.isOpen ? '0' : '-100%')}, 0);
    transition: transform 0.3s ease;
  }

  @media (min-width: ${breakpoints.lg}px) {
    flex: 0 0 ${dimensions.widths.sidebar.lg}px;
    box-shadow: none;
    border-bottom: none;
    background-color: ${colors.grey01};
  }
`;

const WrapperInner = styled('nav')`
  margin-top: ${dimensions.heights.header}px;

  @media (min-width: ${breakpoints.lg}px) {
    width: 200px;
    flex: 1 1 auto;
    z-index: 2;
    height: 100vh;
    overflow-y: auto;
  }
`;

const Header = styled('div')`
  display: flex;
  flex-direction: column;
  position: absolute;
  top: 0;
  left: 0;
  width: 100%;
  height: ${dimensions.heights.header}px;
  padding: 0 24px;
  background-color: ${colors.white};
  border-bottom: 1px solid ${colors.grey02};
  z-index: ${layerIndexes.stickyNav};

  @media (min-width: ${breakpoints.lg}px) {
    border-bottom-color: transparent;
    background-color: ${colors.grey01};
  }
`;

interface HeaderInnerProps {
  hideOnMobile?: boolean;
  hideOnDesktop?: boolean;
}

const HideOnMobile = css`
  @media (max-width: ${breakpoints.lg - 1}px) {
    display: none;
  }
`;

const HideOnDesktop = css`
  @media (min-width: ${breakpoints.lg}px) {
    display: none;
  }
`;

const HeaderInner = styled('div')<HeaderInnerProps>`
  display: flex;
  flex-direction: row;
  flex: 1;
  align-items: center;
  justify-content: space-between;

  ${props => props.hideOnMobile && HideOnMobile}
  ${props => props.hideOnDesktop && HideOnDesktop}
`;

interface FontSizeProps {
  size: ReturnType<typeof determineFontDimensions>;
}

const HomepageLink = styled(Link)<FontSizeProps>`
  color: ${colors.grey09};
  font-size: ${props => props.size.fontSize};
  font-size: ${props => props.size.lineHeight};
  font-weight: ${props => props.size.fontWeight};

  &:hover,
  &:focus {
    color: ${colors.grey09};
    text-decoration: none;
  }
`;

const DocumentationMenu = styled('div')`
  display: flex;
  flex-direction: column;
  padding: 16px 24px;
  border-bottom: 1px solid ${colors.grey02};

  a {
    padding: 8px 0;
    color: ${colors.grey07};

    &:hover,
    &:focus,
    &.active {
      color: ${colors.blue07};
      text-decoration: none;
      outline: none;
    }
  }

  ${HideOnDesktop}
`;

const DocumentationNav = styled('div')`
  display: flex;
  flex-direction: column;
  padding: 24px;
`;

interface NavigationProps {
  title: string;
  navigation?: Edge<MenuNode>[];
  headerMenus?: Edge<HeaderMenuItem>[];
}

function Navigation({ title, navigation, headerMenus }: NavigationProps) {
  const { state, dispatch } = React.useContext(NavigationContext);

  return (
    <Wrapper isOpen={state.isOpen}>
      <Header>
        <HeaderInner hideOnMobile>
          <HomepageLink
            to="/"
            size={determineFontDimensions('heading', 400)}
            onClick={() => dispatch({ type: NavigationActionTypes.CLOSE_DRAWER })}
          >
            {title}
          </HomepageLink>
        </HeaderInner>
        <HeaderInner hideOnDesktop>
          <Heading as="h1" scale={400}>
            Menu
          </Heading>
          <NavButton
            icon="x"
            fill={colors.blue08}
            onClick={() => dispatch({ type: NavigationActionTypes.TOGGLE_DRAWER })}
          >
            Toggle Drawer
          </NavButton>
        </HeaderInner>
      </Header>
      <WrapperInner>
        <DocumentationMenu>
          {headerMenus &&
            headerMenus.map(({ node }) => {
              if (node.external) {
                return (
                  <a key={node.id} href={node.href} target="_blank" rel="noopener noreferrer">
                    {node.label}
                  </a>
                );
              }

              return (
                <Link key={node.id} getProps={isActive} to={node.href}>
                  {node.label}
                </Link>
              );
            })}
        </DocumentationMenu>
        <DocumentationNav onClick={() => dispatch({ type: NavigationActionTypes.TOGGLE_DRAWER })}>
          {navigation &&
            navigation.map(({ node }) => <NavigationMenu key={node.title} menuKey={node.title} node={node} />)}
        </DocumentationNav>
      </WrapperInner>
    </Wrapper>
  );
}

export default Navigation;
