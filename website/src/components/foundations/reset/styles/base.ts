import { css } from 'styled-components';
import { textSizes, colors } from 'components/foundations/variables';

const base = css`
  :root {
    font-size: ${textSizes[300].fontSize}px;
    line-height: ${textSizes[300].lineHeight}px;
  }

  html,
  body,
  #root {
    width: 100%;
    height: 100%;
  }

  a {
    color: ${colors.blue05};
    text-decoration: none;

    &:hover,
    &:focus {
      color: ${colors.indigo04};
      text-decoration: underline;
    }
  }

  img {
    display: block;
    max-width: 100%;
  }

  #root {
    transition: all 0.5s cubic-bezier(0.15, 1, 0.3, 1);
    -webkit-transition: all 0.5s cubic-bezier(0.15, 1, 0.3, 1);

    &.pushed-legend-right {
      transform: translateX(-280px);
    }
  }

  .noscroll {
    overflow: hidden;
  }

  .noselect {
    user-select: none;
  }

  .full-size {
    height: 100%;
    width: 100%;
  }

  .full-size-layout {
    height: 100%;
    min-height: 100vh;
    width: 100%;
  }

  .icon-middle {
    &::before {
      vertical-align: middle;
    }
  }

  .drag-handle {
    cursor: move;
    display: inline-block;

    &::before {
      content: '......';
      display: inline-block;
      width: 10px;
      word-break: break-word;
      white-space: normal;
      letter-spacing: 2px;
      line-height: 4.5px;
      text-align: center;
      height: 18px;
    }
  }

  /* https://github.com/reach/reach-ui/blob/master/packages/skip-nav/styles.css */
  [data-reach-skip-link] {
    border: 0;
    clip: rect(0 0 0 0);
    height: 1px;
    width: 1px;
    margin: -1px;
    padding: 0;
    overflow: hidden;
    position: absolute;
  }

  [data-reach-skip-link]:focus {
    padding: 1rem;
    position: fixed;
    top: 10px;
    left: 10px;
    background: white;
    z-index: 100;
    width: auto;
    height: auto;
    clip: auto;
  }
`;

export default base;
