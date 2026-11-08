#pragma once
#include"ppt.h"
#include<sstream>

// 把每页的 Python 代码合成一个脚本，运行后生成 output.pptx
void exportppt(ppt& p,string filename="generate_ppt.py")
{
	ofstream out(filename);
	out<<"# -*- coding: utf-8 -*-\n";
	out<<"from pptx import Presentation\nfrom pptx.util import Inches\n";
	out<<"from pathlib import Path\n\n";
	out<<"prs = Presentation()\n";
	out<<"prs.slide_width = Inches(13.333)\nprs.slide_height = Inches(7.5)\n\n";
	for(int i=0;i<p.size();i++)
	{
		string code=p.getslide(i)->gethtml();
		if(code.empty())
		{
			out<<"prs.slides.add_slide(prs.slide_layouts[6])\n\n";
			continue;
		}
		istringstream in(code);
		string line;
		while(getline(in,line))
		{
			if(line.find("```")==0)continue;
			out<<line<<'\n';
		}
		out<<"\nadd_slide(prs)\n\n";
	}
	out<<"prs.save(str(Path(__file__).with_name('output.pptx')))\n";
}
