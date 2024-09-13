declare module 'winpeek' {
    export function getActiveWindow(): string | null;
    export function init(options?: { osxRunLoop?: string }): void;
  }
  