    module.exports = {
        content: ['./src/**/*.{vue,js,ts}'],
        plugins: [require('daisyui')],
        // daisyUI config (optional)
        daisyui: {
            themes: [
                {
                    mythemeLight: {
                        "primary": "#1e88e5",
                        "secondary": "#f000b8",
                        "accent": "#1dcdbc",
                        "neutral": "#2b3440",
                        "base-100": "#ffffff",
                        "info": "#3abff8",
                        "success": "#36d399",
                        "warning": "#fbbd23",
                        "error": "#f87272",
                    },
                    mythemeDark: {
                        "primary": "#3a6fff",
                        "secondary": "#73BFB8",
                        "accent": "#34c97c",
                        "neutral": "#161A1D",
                        "base-100": "#3B3E5E",
                        "info": "#81A3F8",
                        "success": "#4BD281",
                        "warning": "#F5AE56",
                        "error": "#DF4358",
                    },

                },
                "dark",
                "light",


            ],
            styled: true,
            base: true,
            utils: true,
            logs: true,
            rtl: false,
            prefix: "",
            darkTheme:"mythemeLight",
        },
    };
