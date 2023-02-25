package org.janksters.ExampleCommonClasses.Drawing;

import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Scanner;

import edu.wpi.first.wpilibj.util.Color;

// See: https://en.wikipedia.org/wiki/Glyph_Bitmap_Distribution_Format

public class BitmapFont {
    public class BitmapFontGlyph {
        /**
         * Nominal width and height of the glyph. 
         */
        protected Dimension dimension;

        /**
         * Baseline offset for the glyph. This represents where the bottom left of the bitmap
         * data rests relative to the origin of the font. For this measure, x is positive to
         * the right, and y is positive up instead of down!
         */
        protected Point baselineOffset;


        /**
         * Left (most significant bit) aligned bitmap of the glyph, starting at the top.
         */
        protected ArrayList<Integer> rawBitmap;

        /**
         * A simplified version of the bitmap. Each entry of the outer ArrayList represents
         * one row. And each entry in the inner ArrayList is one Boolean value representing a
         * pixel for each column in the row.
         */
        protected ArrayList<ArrayList<Boolean>> simplifiedBitmap;

        /**
         * Number of pixels after which the next glyph should be drawn
         */
        protected int glpyhWidth;
        protected char asciiCode;

        protected BitmapFontGlyph(char asciiCode) {
            this.asciiCode = asciiCode;
            rawBitmap = new ArrayList<Integer>();
        }

        protected void addBitmapRow(int row) {
            rawBitmap.add(row);
        }

        public Dimension dimension() {
            return dimension;
        }

        public int glpyhWidth() {
            return glpyhWidth;
        }

        /**
         * Determine the field width of the bitmap. The pixel data starts at (1 << bitWidth()) in the 
         * bitmap array.
         * @return The field width of the bitmap
         */
        public int bitWidth() {
            assert(glpyhWidth <= 24);
            if (glpyhWidth <= 8) {
                return 8;
            } else if (glpyhWidth <= 16) {
                return 16;
            } else if (glpyhWidth <= 24) {
                return 24;
            }

            return 0; // Unsupported
        }

        public ArrayList<ArrayList<Boolean>> bitmap() {
            return simplifiedBitmap;
        }

        /** 
         * Draw the given glyph with a top-left corner at the given coordinate in the drawable context.
         * @param font The font this glyph is part of
         * @param origin The top-left coordinate
         * @param color The foreground color to use
         * @param context The drawing context to use. 
         * @return Returns the x value immediately following the glyph (where to start the next glyph)
         */
        public int draw(BitmapFont font, Point origin, Color color, BitmapDrawingContext context) {
            if (origin.x <= context.width() && origin.x + dimension.width >= 0 &&
                origin.y <= context.height() && origin.y + dimension.height >= 0) {
                // First, determine where the origin is for the font. It's at the left edge of the baseline,
                // which is offset from the bottom left corner of the bounding box by the origin offset.
                Point glyphOrigin = font.origin();
                glyphOrigin.translate(baselineOffset.x, -baselineOffset.y);

                // Glyph origin is relative to top-left, so translate it by the starting position
                glyphOrigin.translate(origin.x, origin.y);

                // Now glyph origin is the bottom left of the bitmap data we want to draw
                for (int pixelDataRow = 0; pixelDataRow < dimension.height; pixelDataRow++) {
                    int y = glyphOrigin.y + pixelDataRow - dimension.height;
                    int rawRowValue = rawBitmap.get(pixelDataRow);
                    for (int pixelDataColumn = 0; pixelDataColumn < dimension.width; pixelDataColumn++) {
                        int mask = (1 << (bitWidth() - 1 - pixelDataColumn));
                        boolean pixelValue = (rawRowValue & mask) != 0;
                        if (pixelValue) {
                            // We need to draw this pixel. The x coordinate is simply x + the origin. The y
                            // value in origin is the bottom of the pixel data, so we need to start drawing
                            // at the top (dimension.height earlier).
                            // MDS: Off by 1 here? 
                            int x = glyphOrigin.x + pixelDataColumn;
                            context.setPixelByXY(x, y, color);
                        }
                    }
                }
            }

            return origin.x + glpyhWidth();
        }
    }

    protected HashMap<Character, BitmapFontGlyph> glyphMap = new HashMap<Character, BitmapFontGlyph>(128);

    /**
     * The font's name
     */
    public String name = "Unknown";

    /**
    * Nominal width and height of the font. Maybe be overridden by the individual glyphs
    */
    protected Dimension dimension;

    /**
     * Origin offset. This is defined as the offset from the bottom left of the font bounding box.
     * For this measure, x positive is to the right, and y positive is down. The origin is nominally
     * where the glyph is positioned on the baseline. For fonts with descenders, the y offset is likely
     * above the bottom of the bounding box. Note that it's possible for an individual glyph to extend
     * to the left of the origin -- for italic fonts?
     */
    protected Point originOffset;


    static public final String kEncoding = "ENCODING";
    static public final String kBitmap = "BITMAP";
    static public final String kStartChar = "STARTCHAR";
    static public final String kEndChar = "ENDCHAR";
    static public final String kGlyphMetrics = "DWIDTH";
    static public final String kFontBoundingBox = "FONTBOUNDINGBOX";
    static public final String kCharacterBoundingBox = "BBX";

    /**
     * Add a new glyph to the font object.
     * @param glyph
     */
    public void addGlyph(BitmapFontGlyph glyph) {
        glyphMap.put(Character.valueOf(glyph.asciiCode), glyph);
    }

    /**
     * Fetch the glyph for the given Character.
     * @param c The Character to fetch the glyph for
     * @return The glyph
     */
    public BitmapFontGlyph glyph(Character c) {
        return glyphMap.get(c);
    }

    /**
     * Fetch the glyph for the given character (ascii encoding).
     * @param c The character to fetch the glyph for
     * @return The glyph
     */
    public BitmapFontGlyph glyph(char c) {
        return glyphMap.get(Character.valueOf(c));
    }

    /**
     * Construct a new font object from the given file
     * @param fontFile The file from which to load font data
     */
    public BitmapFont(File fontFile) {
        name = fontFile.getName();
        load(fontFile);
        System.out.println("Loaded " + numberOfGlyphs() + " glyphs to font " + name);
    }

    /**
     * Get the nominal dimension of the font in pixels
     * @return Nominal width/height of the font
     */
    Dimension dimension() {
        return dimension;
    }

    /**
     * Returns the number of glyphs contained within the font
     * @return Number of glyphs
     */
    int numberOfGlyphs() {
        return glyphMap.size();
    }

    /**
     * Load the given font file into the Font object
     * @param fontFile The font File to load
     */
    protected void load(File fontFile) {
        Scanner fileScanner;
        try {
            fileScanner = new Scanner(fontFile);
        } catch (Exception e) {
            return;
        }
        BitmapFontGlyph currentGlyph = null;    // May be null if we don't care about the glyph
        Boolean inCharacterDefinition = false;  // Are we parsing a glyph
        Boolean inBitmapDefinition = false;     // Are we parsing a glyph's bitmap
        while (fileScanner.hasNextLine()) {
            String line = fileScanner.nextLine();
            if (inBitmapDefinition) {
                if (line.startsWith(kEndChar)) {
                    // Done with this character
                    inCharacterDefinition = inBitmapDefinition = false;
                    if (currentGlyph != null) {
                        assert(currentGlyph.rawBitmap.size() == currentGlyph.dimension.height);

                        addGlyph(currentGlyph);
                        currentGlyph = null;
                    }
                } else {
                    // Must be a bitmap entry
                    if (currentGlyph != null) {
                        currentGlyph.addBitmapRow(Integer.parseUnsignedInt(line, 16));
                    }
                }
            } else if (inCharacterDefinition) {
                if (line.startsWith(kEncoding)) {
                    var lineScanner = new Scanner(line);
                    lineScanner.next(); // Skip the keyword
                    var encoding = (char)lineScanner.nextInt();
                    lineScanner.close();
                    if (encoding < 128) {
                        // We only care about ascii characters. If there are unicode mappings, ignore them
                        currentGlyph = new BitmapFontGlyph(encoding);
                    }
                } else if (line.startsWith(kGlyphMetrics)) {
                    // DWIDTH x y: We want just the x value
                    if (currentGlyph != null) {
                        var lineScanner = new Scanner(line);
                        lineScanner.next(); // Skip over the keyword
                        var glyphWidth = lineScanner.nextInt();
                        lineScanner.close();
                        currentGlyph.glpyhWidth = glyphWidth;
                    }
                } else if (line.startsWith(kBitmap)) {
                    inBitmapDefinition = true;
                } else if (line.startsWith(kCharacterBoundingBox)) {
                    // BBX 4 5 0 0 
                    if (currentGlyph != null) {
                        var lineScanner = new Scanner(line);
                        lineScanner.next(); // Skip over the keyword
                        var width = lineScanner.nextInt();
                        var height = lineScanner.nextInt();
                        var xOffset = lineScanner.nextInt();
                        var yOffset = lineScanner.nextInt();
                        currentGlyph.dimension = new Dimension(width, height);
                        currentGlyph.baselineOffset = new Point (xOffset, yOffset);
                        lineScanner.close();
                    }
                }
            } else if (line.startsWith(kStartChar)) {
                inCharacterDefinition = true;
            } else if (line.startsWith(kFontBoundingBox)) {
                var lineScanner = new Scanner(line);
                lineScanner.next(); // Skip the keyword
                var width = lineScanner.nextInt();
                var height = lineScanner.nextInt();
                var originOffsetX = lineScanner.nextInt();
                var originOffsetY = lineScanner.nextInt();
                dimension = new Dimension(width, height);
                originOffset = new Point(originOffsetX, originOffsetY);
                lineScanner.close();
            }
        }
        fileScanner.close();
    }

    /**
     * Draw the given string starting with an upper left corner at origin of the drawing
     * context. This method does not wrap text or do bounds checks.
     * @param string The string to draw in a single line
     * @param origin Top left starting position
     * @param color Foreground color for the string 
     * @param context The drawing context to use
     * @return The x coordinate immediately following the string
     */
    public int drawString(String string, Point origin, Color color, BitmapDrawingContext context) {
        int nextStartX = origin.x;
        for (int index = 0; index < string.length(); index++) {
            char c = string.charAt(index);
            BitmapFontGlyph glyph = glyph(c);
            if (glyph != null) {
                nextStartX = glyph.draw(this, new Point(nextStartX, origin.y), color, context);
            } else {
                System.out.println("Missing font definition for character: " + (int)c);
            }
        }

        return nextStartX;
    }

    /**
     * Determine the bounds of the given string when drawn with this font.
     * @param string The text that will be drawn
     * @return The dimensions of the string.
     */
    public Dimension bounds(String string) {
        int stringWidth = 0;
        for (int index = 0; index < string.length(); index++) {
            char c = string.charAt(index);
            BitmapFontGlyph glyph = glyph(c);
            stringWidth += glyph.glpyhWidth();
        }

        return new Dimension(stringWidth, dimension.height);
    }

    /**
     * Get the location of the origin relative to the upper left hand corner of the bounding box.
     * @return The font origin on the baseline
     */
    public Point origin() {
        return new Point(originOffset.x, dimension.height + originOffset.y);
    }
}
