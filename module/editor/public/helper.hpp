#pragma once

inline void DrawRectangleVoxelWithOutline(Rectangle rect, int cornerSize, int border, Color color, Color outlineColor) {
    Rectangle out = { rect.x - border, rect.y - border, rect.width + border * 2, rect.height + border * 2 };
    int outCorner = cornerSize + border;
    DrawRectangle(out.x, out.y + outCorner, out.width, out.height - 2 * outCorner, outlineColor);
    DrawRectangle(out.x + outCorner, out.y, out.width - 2 * outCorner, out.height, outlineColor);
    DrawRectangle(rect.x, rect.y + cornerSize, rect.width, rect.height - 2 * cornerSize, color);
    DrawRectangle(rect.x + cornerSize, rect.y, rect.width - 2 * cornerSize, rect.height, color);
}