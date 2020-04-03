class Slider{
	public:
		Slider(double& v, double from, double to, double x, double y, double width, double height);
		double getValue(){return mValue;};
		void setValue(double v){mValue=v;};
		bool hovering(double x, double y);
		void drag(double x, double y);
		void draw();
		void drawActive();
	private:
		double& mValue;
		double mFrom;
		double mTo;
		double mX;
		double mY;
		double mWidth;
		double mHeight;
};