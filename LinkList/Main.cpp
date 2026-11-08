#include<bits/stdc++.h>
#include"ppt.h"
#include"tool.h"
#include"testdata.h"
#include<windows.h>
using namespace std;
int num_of_ppt=0;
LinkList<ppt>user;
void addppt()
{

    cout<<"输入名字：";
    string s;
    getline(cin>>ws,s);

    cout<<"输入功能：";
    string s1;
    getline(cin>>ws,s1);
    ppt ppt_add=ppt(s,s1);
    user.Append(ppt_add);
    num_of_ppt++;
}
void delppt()
{
    cout<<"编号:";
    int n;
    cin>>n;
    user.Go(n);
    user.DeleteCurNode();
    num_of_ppt--;
}
void gotoppt(int n)
{
    user.Go(n);
    ppt& now_ppt=user.CurData();
    while(true)
    {
        cout<<"\n"<<now_ppt.getname()<<"\n";
        for(int i=0;i<now_ppt.size();i++)
            cout<<i<<' '<<now_ppt.getslide(i)->getprompt()<<'\n';
        cout<<"输入页号编辑，新增页扣-1，删除页扣-2，导出脚本扣-3，返回扣-4，生成全部页代码扣-5\n";
        int t;
        cin>>t;
        if(t==-4)return;
        if(t==-1)
        {
            cout<<"输入这页的要求：";
            string s;
            getline(cin>>ws,s);
            slide new_slide;
            new_slide.setprompt(s);
            now_ppt.addslide(new_slide);
        }
        else if(t==-2)
        {
            cout<<"页号：";
            int pos;
            cin>>pos;
            now_ppt.delslide(pos);
        }
        else if(t==-3)
        {
            exportppt(now_ppt);
            cout<<"已导出 generate_ppt.py，在当前目录运行 py -3 generate_ppt.py 生成 output.pptx\n";
        }
        else if(t==-5)
        {
            now_ppt.generateall();
        }
        else
        {
            slide* now_slide=now_ppt.getslide(t);
            while(true)
            {
                cout<<"\n第"<<t<<"页："<<now_slide->getprompt()<<'\n';
                cout<<"修改要求扣1，生成本页扣2，查看代码扣3，返回扣0\n";
                int op;
                cin>>op;
                if(op==0)break;
                if(op==1)
                {
                    cout<<"新的要求：";
                    string s;
                    getline(cin>>ws,s);
                    now_slide->setprompt(s);
                    s="";
                    now_slide->sethtml(s);
                }
                else if(op==2)
                {
                    cout<<"正在生成...\n";
                    now_ppt.generate(t);
                    cout<<"生成完成\n";
                }
                else if(op==3)
                    cout<<now_slide->gethtml()<<'\n';
            }
        }
    }
}
void menu()
{
    while(true)
    {
        cout<<"主界面\n";
        cout<<"ppt:\n";
        for(int i=0;i<num_of_ppt;i++)
        {
            cout<<i<<' ';
            user.Go(i);
            cout<<user.CurData().getname()<<'\n';
        }
        cout<<"前往i号ppt扣i,新增ppt扣-1,删除ppt扣-2,退出扣-3,加载10页测试PPT扣-4\n";
        int t;
        cin>>t;
        if(t==-1)
        {
            addppt();
        }
        else if(t==-2)
        {
            delppt();
        }
        else if(t==-3)
        {
            return;
        }
        else if(t==-4)
        {
            user.Append(testppt());
            num_of_ppt++;
            cout<<"已加载10页测试PPT，编号："<<num_of_ppt-1<<'\n';
        }
        else
        {
            gotoppt(t);
        }
    }
}
int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    menu();
}