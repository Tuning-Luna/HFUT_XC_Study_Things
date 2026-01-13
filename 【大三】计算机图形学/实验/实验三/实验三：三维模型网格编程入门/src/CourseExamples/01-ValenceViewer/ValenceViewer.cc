//=============================================================================
//                                                
//   Code framework for the lecture
//
//   "Surface Representation and Geometric Modeling"
//
//   Mark Pauly, Mario Botsch, Balint Miklos, and Hao Li
//
//   Copyright (C) 2007 by  Applied Geometry Group and 
//							Computer Graphics Laboratory, ETH Zurich
//                                                                         
//-----------------------------------------------------------------------------
//                                                                            
//                                License                                     
//                                                                            
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the GNU General Public License
//   as published by the Free Software Foundation; either version 2
//   of the License, or (at your option) any later version.
//   
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//   
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 51 Franklin Street, Fifth Floor, 
//   Boston, MA  02110-1301, USA.
//                                                                            
//=============================================================================
//=============================================================================
//
//  CLASS ValenceViewer - IMPLEMENTATION
//
//=============================================================================


//== INCLUDES =================================================================

#include "ValenceViewer.hh"
#include <vector>
#include <float.h>



//== IMPLEMENTATION ========================================================== 


ValenceViewer::
ValenceViewer(const char* _title, int _width, int _height)
	: MeshViewer(_title, _width, _height)
{
	mesh_.request_vertex_colors();
	mesh_.add_property(valence_handle_);  // 注册valence属性

	add_draw_mode("Vertex Valences");

}


//-----------------------------------------------------------------------------


ValenceViewer::
~ValenceViewer()
{
}

//-----------------------------------------------------------------------------

bool
ValenceViewer::
open_mesh(const char* _filename)
{
	// load mesh
	if (MeshViewer::open_mesh(_filename))
	{
		// compute vertex valence and color coding
		calc_valences();
		color_coding();

		glutPostRedisplay();
		return true;
	}
	return false;
}


//-----------------------------------------------------------------------------


void ValenceViewer::calc_valences()
{
	// 1. 获取迭代器，开始遍历网格所有顶点
	for (Mesh::VertexIter v_it = mesh_.vertices_begin(); v_it != mesh_.vertices_end(); ++v_it) {
		Mesh::VertexHandle vh = v_it;

		int valence = 0;
		// 2. 关键：vv_iter 访问的就是 N(xi)，即 1-邻域
		for (Mesh::VertexVertexIter vv_it = mesh_.vv_iter(vh); vv_it; ++vv_it) {
			++valence; // 每发现一个邻居，度数加 1
		}

		// 3. 将结果存入实验要求的自定义属性
		mesh_.property(valence_handle_, vh) = valence;
	}

	// 打印示例
	std::cout << "Sample vertex valences:" << std::endl;
	int count = 0;
	for (Mesh::VertexIter v_it = mesh_.vertices_begin(); v_it != mesh_.vertices_end() && count < 5; ++v_it, ++count)
	{
		Mesh::VertexHandle vh = v_it;
		int idx = vh.idx();
		int val = mesh_.property(valence_handle_, vh);
		std::cout << "Vertex " << idx << ": valence = " << val << std::endl;
	}
}


//-----------------------------------------------------------------------------


void ValenceViewer::color_coding()
{
	// 如果网格中没有顶点，直接返回，避免后续处理空数据
	if (mesh_.n_vertices() == 0) return;

	// 初始化最小度和最大度变量
	// min_val 设为整数最大值，max_val 设为整数最小值，确保在第一次比较时能被正确替换
	int min_val = INT_MAX, max_val = INT_MIN;

	// 遍历网格中的所有顶点
	for (Mesh::VertexIter v_it = mesh_.vertices_begin(); v_it != mesh_.vertices_end(); ++v_it)
	{
		// 获取当前顶点的句柄（Handle）
		Mesh::VertexHandle vh = v_it;

		// 从之前存储的自定义属性 valence_handle_ 中读取该顶点的度数
		int val = mesh_.property(valence_handle_, vh);

		min_val = std::min(min_val, val);
		max_val = std::max(max_val, val);
	}

	std::cout << "Valence range: [" << min_val << ", " << max_val << "]" << std::endl;

	float range = static_cast<float>(max_val - min_val);
	if (range <= 0) range = 1.0f; // 防止除零

	// 方案：手动限制范围，让 6 处于中间
	float min_display = 3.0f;
	float max_display = 9.0f; // 超过 9 的都当做 9 处理（红色）
	float range2 = max_display - min_display;

	// 再次遍历所有顶点，根据度数为每个顶点计算并设置颜色
	for (Mesh::VertexIter v_it = mesh_.vertices_begin(); v_it != mesh_.vertices_end(); ++v_it)
	{
		// 获取当前顶点的句柄
		Mesh::VertexHandle vh = v_it;

		// 获取该顶点的度数
		int val = mesh_.property(valence_handle_, vh);
		float t = (val - min_display) / range2;

		// 边界限制（Clamp）：确保 t 不会超出 [0, 1] 范围
		t = std::max(0.0f, std::min(1.0f, t));

		// 定义一个 3 字节的向量来存储 RGB 颜色 (unsigned char)
		OpenMesh::Vec3uc color;

		// 颜色插值逻辑：采用“蓝-绿-红”渐变
		if (t < 0.5f)
		{
			// 第一阶段：从蓝色 (t=0) 过渡到绿色 (t=0.5)
			float s = t * 2.0f;           // 将 [0, 0.5] 映射到 [0, 1] 的局部比例 s
			color[0] = 0;                 // 红色分量为 0
			color[1] = static_cast<unsigned char>(s * 255);        // 绿色分量逐渐增加
			color[2] = static_cast<unsigned char>((1.0f - s) * 255); // 蓝色分量逐渐减小
		}
		else
		{
			// 第二阶段：从绿色 (t=0.5) 过渡到红色 (t=1.0)
			float s = (t - 0.5f) * 2.0f;  // 将 [0.5, 1] 映射到 [0, 1] 的局部比例 s
			color[0] = static_cast<unsigned char>(s * 255);        // 红色分量逐渐增加
			color[1] = static_cast<unsigned char>((1.0f - s) * 255); // 绿色分量逐渐减小
			color[2] = 0;                 // 蓝色分量为 0
		}

		// 将计算好的颜色应用到当前顶点，供 GUI 渲染使用
		mesh_.set_color(vh, color);
	}

	// 统计度数分布
	std::map<int, int> hist;
	for (Mesh::VertexIter v_it = mesh_.vertices_begin(); v_it != mesh_.vertices_end(); ++v_it)
	{
		Mesh::VertexHandle vh = v_it;
		int val = mesh_.property(valence_handle_, vh);
		hist[val]++;
	}

	std::cout << "Valence histogram:\n";
	for (const auto& kv : hist)
	{
		std::cout << "  " << kv.first << ": " << kv.second << " vertices\n";
	}
}


//-----------------------------------------------------------------------------


void
ValenceViewer::
draw(const std::string& _draw_mode)
{

	if (indices_.empty())
	{
		MeshViewer::draw(_draw_mode);
		return;
	}

	if (_draw_mode == "Vertex Valences")
	{

		glDisable(GL_LIGHTING);
		glShadeModel(GL_SMOOTH);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		GL::glVertexPointer(mesh_.points());
		GL::glNormalPointer(mesh_.vertex_normals());
		GL::glColorPointer(mesh_.vertex_colors());
		glDepthRange(0.01, 1.0);
		glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, &indices_[0]);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glColor3f(0.1, 0.1, 0.1);
		glEnableClientState(GL_VERTEX_ARRAY);
		GL::glVertexPointer(mesh_.points());
		glDrawBuffer(GL_BACK);
		glDepthRange(0.0, 1.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDepthFunc(GL_LEQUAL);
		glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, &indices_[0]);
		glDisableClientState(GL_VERTEX_ARRAY);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDepthFunc(GL_LESS);
	}

	else MeshViewer::draw(_draw_mode);
}


//=============================================================================
