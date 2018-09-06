#pragma once

#include <vector>

namespace Core {
    using namespace std;

    /**
     * @brief work 计算每个管道的流速
     * @param n 网格边长
     * @param length 管道的长度数组，若相应数值为0则表示该管道不存在
     * @param area 管道横截面积
     * @param i1 1号输入口所在列（0 <= i1）
     * @param i2 2号输入口所在列（i1 < i2 < n）
     * @param o1 1号输出口所在列（0 <= o1）
     * @param o2 2号输出口所在列（o1 < o2）
     * @param o3 3号输出口所在列（o2 < o3 < n）
     * @param initSpeed 输入口的初始流速
     * @param concen1 1号输入口的初始浓度
     * @param concen2 2号输入口的初始浓度
     * @param speed 最终求出来的流速，如果是正数，表示从左到右（从上到下）的方向，如果是负数，表示从右到左（从下到上）的方向
     * @param concen 最终求出来的浓度
     *
     * 用高斯消元解方程组，求出流速；再根据电势排序（拓扑序），求出浓度
     */
    extern void work(int n, const vector<double> &length, const vector<double> &area, int i1, int i2, int o1, int o2, int o3, double initSpeed, double concen1, double concen2, vector<double> &speed, vector<double> &concen);
}
