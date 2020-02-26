#pragma once
void DrawLine(double x1, double y1, double x2, double y2);
void DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3);
void DrawCircle(double x1, double y1, double radius);
enum viewtype{top_view, perspective_view, rat_view};
extern viewtype current_view;