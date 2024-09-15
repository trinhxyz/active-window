declare namespace winpeek {
  /**
   * Result returned by `getActiveWindow`.
   * A string with the name of the focused window, or `null` if no window is focused.
   */
  type Result = string | null;
}

declare const winpeek: {
  /**
   * Initializes any platform-specific settings or permissions.
   * This function must be called before `getActiveWindow`.
   *
   * @example
   * ```
   * import { initialize } from 'winpeek';
   *
   * initialize();
   * ```
   */
  initialize: () => void;

  /**
   * Get the name of the currently focused window as a string or `null` if no window is focused.
   * This function should be called after `initialize`.
   *
   * @example
   * ```
   * import { getActiveWindow, initialize } from 'winpeek';
   *
   * initialize();
   *
   * const windowName = await getActiveWindow();
   * console.log(windowName); // Logs the name of the active window or `null`
   * ```
   */
  getActiveWindow: () => Promise<winpeek.Result>;
};

export = winpeek;
