import React from 'react';
import styled from 'styled-components';
import { Link } from 'gatsby';

import { MenuNode } from 'interfaces/nodes';
import { Heading, Box } from 'components/foundations';
import { colors, space } from 'components/foundations/variables';
import { isActive } from 'utils/helpers';

interface NavigationMenuProps {
  node: MenuNode;
  menuKey: string;
}

interface ToggleableProps {
  isOpen?: boolean;
}

const ToggleMenu = styled('ul')<ToggleableProps>`
  list-style-type: none;
  margin: 0 -${space.xs}px;
  padding: 0;
  transition: all 0.3s ease;
`;

const ToggleMenuList = styled('li')`
  margin: 0;
  font-size: 85%;
  color: ${colors.grey07};

  a {
    display: block;
    padding: ${space.xs}px;
    border: 2px solid transparent;
    border-radius: 2px;
    color: ${colors.grey07};

    &:hover,
    &:focus {
      background-color: ${colors.grey02};
      color: ${colors.grey07};
      text-decoration: none;
    }

    &:focus {
      outline: none;
      background-color: ${colors.blue01};
      border-color: ${colors.blue05};
    }

    &.active {
      color: ${colors.grey07};
      background-color: ${colors.blue01};
      border-color: transparent;
    }
  }
`;

const NavigationMenu: React.FC<NavigationMenuProps> = ({ node }) => {
  return (
    <Box mb="md">
      <Heading as="h3" scale={100} color="grey04" mb="sm">
        {node.title}
      </Heading>
      <ToggleMenu>
        {node.items.map(item => (
          <ToggleMenuList key={item.id}>
            <Link to={item.slug} getProps={isActive}>
              {item.title}
            </Link>
          </ToggleMenuList>
        ))}
      </ToggleMenu>
    </Box>
  );
};

export default React.memo(NavigationMenu);
