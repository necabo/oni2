/*
 * ThemeStoreConnector.re
 *
 * This connector handles loading themes and tokenThemes.
 */

open Oni_Core;
open Oni_Model;
open Oni_Syntax;

let start = (setup: Setup.t) => {
  let defaultThemePath =
    setup.bundledExtensionsPath ++ "/onedark-pro/themes/OneDark-Pro.json";

  let loadThemeByPathEffect = themePath =>
    Isolinear.Effect.createWithDispatch(
      ~name="theme.loadThemeByPath", dispatch =>
      Log.perf("theme.load", () => {
        let themeJson = Yojson.Safe.from_file(themePath);

        let tokenColorsJson =
          Yojson.Safe.Util.member("tokenColors", themeJson);
        let textMateTheme =
          Textmate.Theme.of_yojson(
            ~defaultBackground="#2F3440",
            ~defaultForeground="#DCDCDC",
            tokenColorsJson,
          );

        let tokenTheme = TokenTheme.create(textMateTheme);

        dispatch(Actions.SetTokenTheme(tokenTheme));
      })
    );

  let updater = (state: State.t, action: Actions.t) => {
    switch (action) {
    | Actions.Init => (state, loadThemeByPathEffect(defaultThemePath))
    | Actions.LoadThemeByPath(themePath) => (
        state,
        loadThemeByPathEffect(themePath),
      )
    | _ => (state, Isolinear.Effect.none)
    };
  };

  updater;
};
