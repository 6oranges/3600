class Slider{
	public:
		Slider(double from, double to, double x, double y, double width, double height, double* v=nullptr);
		void setValue(double* v){mValue=v;};
		bool hovering(double x, double y);
		void drag(double x, double y);
		void draw();
		void drawActive();
	private:
		double* mValue;
		double mFrom;
		double mTo;
		double mX;
		double mY;
		double mWidth;
		double mHeight;
};