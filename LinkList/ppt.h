#include"LinkList.h"
class slide {
private:
	string html;
	string prompt;
public:
	slide():html(),prompt(){}
	void setprompt(string &s)
	{
		prompt = s;
	}
	void sethtml(string& s)
	{
		html = s;
	}
	string& getprompt()
	{
		return prompt;
	}
	string& gethtml(string& s)
	{
		return html;
	}
};
class ppt{
private:
	int num_of_slides;
	LinkList<slide>slides;
public:
	ppt():num_of_slides(0),slides(){}
	ppt(const ppt&  b):num_of_slides(b.num_of_slides),slides(b.slides){}
	slide* getslide(int pos) {
		if (pos <0 || pos > =num_of_slides) return nullptr;  // 指针可以返回nullptr
		slides.Go(pos);
		return &slides.CurData();
	}
	void setslide(int pos,slide s)
	{
		if (pos <0 || pos > =num_of_slides)
		{
			cout << "mjmj" << endl;
			return;
		}
		slides.Go(pos);
		slides.ModifyData(s);
	}
	void addslide(slide s)
	{
		slides.Append(s);
		num_of_slides++;
	}
	void delslide(int n)
	{
		if (n == -1)return;
		slides.Go(n);
		slides.DeleteCurNode();
		num_of_slides--;
	}
};