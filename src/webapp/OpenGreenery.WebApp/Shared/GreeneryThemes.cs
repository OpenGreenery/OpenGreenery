using System.Collections.Generic;
using MudBlazor;

namespace OpenGreenery.WebApp.Shared
{
    public class NamedMudTheme : MudTheme
    {
        public string Name { get; init; }
    }

    public static class GreeneryThemes
    {
        public static NamedMudTheme Light = new NamedMudTheme
        {
            Name = "light",
            Palette = new Palette
            {
                Primary = Colors.Green.Default,
            },
        };

        public static NamedMudTheme Dark = new NamedMudTheme
        {
            Name = "dark",
            Palette = new Palette
            {
                Primary = Colors.Green.Darken2,
                Black = "#27272f",
                Background = "#32333d",
                BackgroundGrey = "#27272f",
                Surface = "#373740",
                DrawerBackground = "#27272f",
                DrawerText = "rgba(255,255,255, 0.50)",
                DrawerIcon = "rgba(255,255,255, 0.50)",
                AppbarBackground = "#27272f",
                AppbarText = "rgba(255,255,255, 0.70)",
                TextPrimary = "rgba(255,255,255, 0.70)",
                TextSecondary = "rgba(255,255,255, 0.50)",
                ActionDefault = "#adadb1",
                ActionDisabled = "rgba(255,255,255, 0.26)",
                ActionDisabledBackground = "rgba(255,255,255, 0.12)",
                Divider = "rgba(255,255,255, 0.12)",
                DividerLight = "rgba(255,255,255, 0.06)",
                TableLines = "rgba(255,255,255, 0.12)",
                LinesDefault = "rgba(255,255,255, 0.12)",
                LinesInputs = "rgba(255,255,255, 0.3)",
                TextDisabled = "rgba(255,255,255, 0.2)",
            },
        };

        public static IReadOnlyDictionary<string, NamedMudTheme> Instances = new Dictionary<string, NamedMudTheme>
        {
            { Light.Name, Light },
            { Dark.Name, Dark },
        };
    }
}