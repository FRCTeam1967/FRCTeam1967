package org.janksters.ExampleCommonClasses.Drawing;

import java.util.HashMap;

public class BitmapFontManager {
    private HashMap<String, BitmapFont> fonts = new HashMap<String, BitmapFont>();

    public BitmapFontManager() {
    }

    /**
     * Add a font to the FontManager
     * @param font The font to add
     */
    public void addFont(BitmapFont font) {
        fonts.put(font.name, font);
    }

    /**
     * Get a font from the FontManager by name
     * @param name The name of the font to fetch
     */
    public BitmapFont getFont(String name) {
        return fonts.get(name);
    }
}
