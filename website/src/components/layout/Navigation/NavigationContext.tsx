import * as React from 'react';
import { $Values } from 'utility-types';

interface NavigationState {
  isOpen: boolean;
}

interface NavigationContextValue {
  state: NavigationState;
  dispatch: React.Dispatch<any>;
}

const initialState = {
  isOpen: false
};

const NavigationActionTypes = {
  RESET: 'NavigationContext/RESET',
  OPEN_DRAWER: 'NavigationContext/OPEN_DRAWER',
  CLOSE_DRAWER: 'NavigationContext/CLOSE_DRAWER',
  TOGGLE_DRAWER: 'NavigationContext/TOGGLE_DRAWER'
};

// The only way TypeScript allows us to make a type-safe context value.
const NavigationContext = React.createContext<NavigationContextValue>({} as NavigationContextValue);

interface NavigationActions {
  type: $Values<typeof NavigationActionTypes>;
}

const reducer: React.Reducer<NavigationState, NavigationActions> = (state, action) => {
  switch (action.type) {
    case NavigationActionTypes.RESET:
      return initialState;
    case NavigationActionTypes.OPEN_DRAWER:
      return { ...state, isOpen: true };
    case NavigationActionTypes.CLOSE_DRAWER:
      return { ...state, isOpen: false };
    case NavigationActionTypes.TOGGLE_DRAWER:
      return { ...state, isOpen: !state.isOpen };
    default:
      return state;
  }
};

function NavigationContextProvider(props: { children: React.ReactNode }) {
  const [state, dispatch] = React.useReducer(reducer, initialState);
  const value = { state, dispatch };

  return <NavigationContext.Provider value={value}>{props.children}</NavigationContext.Provider>;
}

const NavigationContextConsumer = NavigationContext.Consumer;

export { NavigationContext, NavigationActionTypes, NavigationContextProvider, NavigationContextConsumer };
