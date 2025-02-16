import { javascriptPart } from './jsPart';

class TypeScriptPart {
  constructor(value: number) {
    console.log('I\'m a TypeScript class constructor I\'m sending',
    value,
    'to JavaScript and I received', javascriptPart(value));
  }
}

new TypeScriptPart(10);