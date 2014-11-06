BGL == Blender OpenGL Wrapper
=============

Blender下的BGL；其实是Blender的OpenGL wapper；OpenGL 是什么？OMG，这个问题其实很重要，因为 OpenGL 是让你的手机，Linux或 Mac 显示三维对象的核心，没有它什么三维都显示不了，更不谈玩游戏了；那为啥没有Windows？因为 Windows 和 Xbox 用的是商业味很重的 DirectorX，但其实它们也借鉴了 OpenGL，GL 意思是计算机图形学，一门很深奥的学问，是数学三维和四维可视化的产物。

注意，这不是一份给完全初学Blender 的教程，这不是一份完成的教程，是针对希望做些 Hacker 的 Blender 爱好者分享的心得，是计划中的某一章节。

:::::::BGL via BGE::::::::

BGE == Blender Game Engine；

BGL 虽然是 Blender 显示三维物体的基础，但要了解其代码，最直接方便的方法还是通过运行 Blender Game Engine，当然这有待讨论，因为从 Blender 脚本编辑器里直接运行也是可以的，只是显示方面的代码.......

------Blender的BGE------


BGE为Blender的游戏引擎模式，该游戏引擎依附于Blender整体编辑器，。。。。可通过BGE的模块设置来实现


本例包括两个部分：
第一部分，是对所创建的三角形进行移动，缩放与旋转；
第二部分，是了解在坐标转换的三个重要阶段：
设立矩阵空间(正则空间)，
设立梯形空间（Frustum）及投影空间（正交的Orthogonal或投影的Perspective），
设立视口空间（View-port ）
本例子要求通过了解在VC++环境下OpenGL代码的执行，从而能够设计编写在Blender的BGE环境下的代码


第一步，了解基本的 OpenGL，

首先IDE环境设置:

在VC++环境下（Visual Studio略同），进行工程设置，加入openGL32.lib glut32.lib；如果是在Mac的Xcode环境下则需要加入OpenGL.framework与GLUT.framework（具体配置下此文档完善）；在工程下新建C++ Source file即CPP文件，加入以下前缀后VC++环境会自动导入外部援引头文件glut.h

================C++ OpenGL工程的图形学解释================

include<gl/glut.h>//这里表示导入GLUT
include<stdlib.h>//这里表示标准库导入

/// 注意，GLUT包括了GLU等亲GL的实用库 ///
/// 其次，在VC++环境下的主干程序： ///

int main(int argc,char** argv){ 
glutInit(&argc,argv);//GLUT主程序 glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);//GLUT显示模式设置 glutInitWindowSize(500,500);//windows环境下窗口大小 glutInitWindowPosition(100,100);//windows环境下窗口位置 glutCreateWindow("Translation");//windows环境下窗口名称“引号内名字自定” 
init(500,500);//启动函数，自设函数，自定义两参数 glutDisplayFunc(display);//显示并启动绘制内容，调用自定绘制函数 glutMainLoop();//主循环 return 0;//返回不停 } 

/// 注意主干程序非openGL绘制内容的核心是GLUT运行骨架，在Blender的python环境下不需要;  以下为VC++下OpenGL绘制三角形： /// 

void Draw_triangle(void) { //函数名自定为Draw_triangle() glLineWidth(3.0); //线宽参数预设 
glBegin(GL_LINE_STRIP); //glBegin与glEnd间绘制对象，对象类型为线 //glBegin(GL_TRIANGLES); //若想绘制对象为三角面可启用，并隐去上面两行 glVertex3i(1,0,0); 
glVertex3i(0,1,0); 
glVertex3i(-1,0,0); 
glVertex3i(1,0,0); //最后一个点（整数模式为i），注意如果是三角面类型则不必，否则四个点才能画三条线 

glEnd(); 

glFlush();//时限内绘制终止，用于刷新，VC++环境必须，BGE也需要 
} 
/// 接A，本教程部分将继续介绍坐标转换和导入Blender的BGE；
以下为自设启动函数，并传递参数w与h： /// 

void init(int w, int h) { 
//Basic View==change to modelview matrix to prepare for draw this scene 基本视图为改变矩阵空间并绘制场景

glMatrixMode(GL_MODELVIEW);//矩阵模式：正则空间视图 
glLoadIdentity();//转换为4X4标准空间 glOrtho(-2.0,2.0,-2.0,2.0,-2.0,2.0);//正交空间2倍同步放大，也就是空间内对象缩小一倍 ///以上为基本绘制，流程主要为矩阵空间>可识别矩阵空间（4X4矩阵）>投影（默认略）>视口（默认）

//Projecton View==set projection matrix设为透视视图

glMatrixMode(GL_PROJECTION);//矩阵模式：投影透视视图
glLoadIdentity(); 
//glFrustum(-1.0,1.0,-1.0,1.0,1.0,10.0);//此为梯形投影空间，（上，下，左，右，近，远） 

gluPerspective(45.0,(GLfloat)w/(GLfloat)h,1.0,150.0);//此为透视空间与上一行梯形投影空间可换，（透视角度，宽高比，近，远），宽高参数由外传内


///以上为投影绘制，可覆盖基本绘制，流程主要为矩阵空间>可识别矩阵空间（4X4矩阵）>投影（选择梯形或透视）>视口（默认）///

//Viewport View 

glViewport(0,0, (GLsizei)w, (GLsizei)h);

//Windows' 宽高参数由外传内 

} 
///此为可识别矩阵空间为4X4，即，X，Y，Z，w，4X4空间易于处理点积（内积）与叉积（外积）等线性运算，除了缩放，移动和旋转，无法在不改变3X3矩阵性质的前提下实现，为了简化运算引入4X4: 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1
=========================================================

VC++下完整代码:
include<gl/glut.h>
include<stdlib.h>
void Draw_triangle(void) { glLineWidth(3.0); glBegin(GL_LINE_STRIP); //glBegin(GL_TRIANGLES); glVertex3i(1,0,0); glVertex3i(0,1,0); glVertex3i(-1,0,0); glVertex3i(1,0,0); glEnd(); glFlush(); }
void display(void) { glClear(GL_COLOR_BUFFER_BIT); glColor3f(1.0,1.0,1.0);// Draw_triangle();//Original
glColor3f(1.0,0.0,0.0);//Red glTranslatef(-0.2,-0.2,-6.0);//X,Y,Z translating! Draw_triangle();//Moved
glColor3f(0.0,1.0,0.0);//Green glTranslatef(-0.2,-0.2,-1.5);//X,Y,Z translating! glScalef(2.0,2.0,2.0);//X,Y,Z translated and scaling! Draw_triangle();//Scaled
glColor3f(0.0,0.0,1.0);//Blue glTranslatef(-0.2,-0.2,-0.5);//X,Y,Z translating! glRotatef(30,0.0,1.0,0.0);//X,Y,Z Rotating! Draw_triangle();//Rotated
glutSwapBuffers(); }
void init(int w, int h) { //Basic View==change to modelview matrix to prepare for draw this scene glMatrixMode(GL_MODELVIEW); glLoadIdentity(); glOrtho(-2.0,2.0,-2.0,2.0,-2.0,2.0);
//Projecton View==set projection matrix glMatrixMode(GL_PROJECTION); glLoadIdentity(); //glFrustum(-1.0,1.0,-1.0,1.0,1.0,10.0); gluPerspective(45.0,(GLfloat)w/(GLfloat)h,1.0,150.0);
//Viewport View glViewport(0,0, (GLsizei)w, (GLsizei)h);//Windows' w and h }
int main(int argc,char** argv) { glutInit(&argc,argv); glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB); glutInitWindowSize(500,500); glutInitWindowPosition(100,100); glutCreateWindow("Translation"); init(500,500); glutDisplayFunc(display); glutMainLoop(); return 0; }


Blender引入指导：

1.参数针对空轴对象，空轴对象上加“总是”触发器，并添加“Python”控制器，且指到新建Text 
2.Text内,导入BGE及BGL模块： 
import bge import bgl from bge import logic from bge import render #自定sce获得场景定位，注意Python结尾无“；”号，且缩进表从属关系，所以无“{”，“ }"！ sce=bge.logic.getCurrentScene() #def为定义函数，冒号结构类C形式！

def Draw_triangle():
    #这里请添加bgl的OpenGL代码
def init():
    #这里请添加bgl的OpenGL代码
def display():
    #这里请添加bgl的OpenGL代码
以下为主结构：
def Draw():
    sce.post_draw=[init,display]
     #post_draw为场景类（Scene）所有的绘制方式，绘制为列表
执行主结构
Draw()

问题：在BGE中以下代码应是怎样的？
glBegin(GL_LINE_STRIP);
答案：bgl.glBegin(bgl.GL_LINE_STRIP)

通过对BGL的了解将下列代码在BGE中执行
以下为自设display()函数,该函数调用了自设Draw_triangle()函数，并规定了绘制方法，即原始绘制，移动，移动并缩放，移动并旋转的绘制方法： /// void display(void) { glClear(GL_COLOR_BUFFER_BIT);//内存设色清空 glColor3f(1.0,1.0,1.0);//色彩设置，RBG类型，1.0==255 Draw_triangle();//调用自设函数绘制原始三角形
glColor3f(1.0,0.0,0.0);//设为红 glTranslatef(-0.2,-0.2,-6.0);//移动XYZ参数为浮点型在Translate后缀f Draw_triangle();//绘制移动后对象
glColor3f(0.0,1.0,0.0);//为绿 glTranslatef(-0.2,-0.2,-1.5);//X,Y,Z translating! glScalef(2.0,2.0,2.0);//在XYZ方向同时放大2倍 Draw_triangle();//移动放大后绘制，注意此次移动会基于上次移动的位置，因为调用同一绘制函数而未清理内存
glColor3f(0.0,0.0,1.0);//为蓝 glTranslatef(-0.2,-0.2,-0.5);//X,Y,Z translating! glRotatef(30,0.0,1.0,0.0);//WXYZ轴向旋转，W为旋转角度，XYZ构成基于对象原点的向量轴 Draw_triangle();//绘制移动旋转后对象
glutSwapBuffers();//绘制层刷新 } ///