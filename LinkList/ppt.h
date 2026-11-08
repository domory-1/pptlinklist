#pragma once
#include"ai.h"
#include"LinkList.h"
using namespace std;
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
	string& gethtml()
	{
		return html;
	}
};
class ppt{
private:
	int num_of_slides;
	string systemprompt,name;
	LinkList<slide>slides;
public:
	ppt(string name1="",string s=""):num_of_slides(0),systemprompt(s),name(name1),slides(){}
	ppt(const ppt&  b):num_of_slides(b.num_of_slides),systemprompt(b.systemprompt),name(b.name),slides(b.slides){}
	slide* getslide(int pos) {
		if (pos <0 || pos >=num_of_slides) return nullptr;  // 指针可以返回nullptr
		slides.Go(pos);
		return &slides.CurData();
	}
	string getname(){
		return name;
	}
	int size(){
		return num_of_slides;
	}
	void setslide(int pos,slide s)
	{
		if (pos <0 || pos >=num_of_slides)
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
	void generate(int n)
	{
		slides.Go(n);
		slide& slide_n=slides.CurData();
		string prompt=slide_n.getprompt();
		string s=systemprompt+"\n请用 python-pptx 生成一页真正的 PPT。"
			"只返回 Python 代码，不要 Markdown 标记或解释。"
			"代码必须定义 def add_slide(prs): 函数，在函数里向已有的 prs 添加且只添加一页。"
			"prs 已经设为 13.333 x 7.5 英寸。不要创建 Presentation，不要保存文件，不要调用函数。"
			"需要的 import 放在函数内，不要依赖外部图片、文件或网络。";
		string html=model(s,prompt);
		slide_n.sethtml(html);
	}
	void generateall()
	{
		for(int i=0;i<num_of_slides;i++)
		{
			cout<<"正在生成第"<<i+1<<"/"<<num_of_slides<<"页..."<<endl;
			generate(i);
			cout<<"第"<<i+1<<"页生成完成"<<endl;
		}
		cout<<"全部页面代码生成完成"<<endl;
	}
};