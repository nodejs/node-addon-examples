export class ObjectWrapDemo {
    constructor(name: string);
    /**
     * return a string like "Hello, ${str}\nI am ${name}\n"
     * @param str greeting string
     */
    greet(str: string): string;
}