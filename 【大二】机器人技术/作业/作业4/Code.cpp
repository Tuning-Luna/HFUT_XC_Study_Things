#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


// 数据样本类
class Sample {
public:
    map<string, string> _attributes;
    string _label;
};

//得到熵
double getEntropy(const vector<Sample>& samples) {
    map<string, int> labelCount;
    for (const auto& sample : samples) {
        ++labelCount[sample._label];
    }

    double E = 0.0; //熵
    for (const auto& count : labelCount) {
        double p = (double)count.second / samples.size();
        E -= p * log2(p);
    }
    return E;
}

//按照某个属性划分数据集
vector<vector<Sample>> splitSamples(const vector<Sample>& samples, const string& _attribute) {
    map<string, vector<Sample>> split;
    for (const auto& sample : samples) {
        split[sample._attributes.at(_attribute)].push_back(sample);
    }

    vector<vector<Sample>> splitResult;
    for (const auto& subset : split) {
        splitResult.push_back(subset.second);
    }
    return splitResult;
}

//得到信息增益
double getGain(const vector<Sample>& samples, const string& _attribute) {
    double E = getEntropy(samples);

    vector<vector<Sample>> splitResult = splitSamples(samples, _attribute);
    double newE = 0.0;
    for (const auto& subset : splitResult) {
        newE += static_cast<double>(subset.size()) / samples.size() * getEntropy(subset);
    }

    return E - newE;
}

//选择信息增益最高的属性
string chooseBestAttribute(const vector<Sample>& samples, const set<string>& _attributes) {
    string bestAttribute;
    double maxGain = -1;
    for (const auto& _attribute : _attributes) {
        double gain = getGain(samples, _attribute);
        if (gain > maxGain) {
            maxGain = gain;
            bestAttribute = _attribute;
        }
    }
    return bestAttribute;
}

class TreeNode {
public:
    string _attribute;
    map<string, TreeNode*> _children;
    string _label;

    TreeNode() : _attribute(""), _label("") {}
    ~TreeNode() {
        for (auto& child : _children) {
            delete child.second;
        }
    }
};

//构建决策树
TreeNode* buildTree(const vector<Sample>& samples, set<string> _attributes) {
    // 如果所有样本的标签相同，返回叶节点
    string firstLabel = samples[0]._label;
    bool allSame = all_of(samples.begin(), samples.end(), [&firstLabel](const Sample& s) {
        return s._label == firstLabel;
        });
    if (allSame) {
        TreeNode* leaf = new TreeNode();
        leaf->_label = firstLabel;
        return leaf;
    }

    // 如果没有可用的属性，返回叶节点（选择样本中出现最多的标签）
    if (_attributes.empty()) {
        TreeNode* leaf = new TreeNode();
        map<string, int> labelCount;
        for (const auto& sample : samples) {
            labelCount[sample._label]++;
        }
        leaf->_label = max_element(labelCount.begin(), labelCount.end(),
            [](const pair<string, int>& a, const pair<string, int>& b) {
                return a.second < b.second;
            })->first;
        return leaf;
    }

    string bestChoice = chooseBestAttribute(samples, _attributes);
    TreeNode* node = new TreeNode();
    node->_attribute = bestChoice;
    _attributes.erase(bestChoice);
    vector<vector<Sample>> subsets = splitSamples(samples, bestChoice);
    for (const auto& subset : subsets) {
        TreeNode* child = buildTree(subset, _attributes);
        node->_children[subset[0]._attributes.at(bestChoice)] = child;
    }

    return node;
}

// 打印决策树（前序遍历）
void printTree(TreeNode* node, const string& prefix = "") {
    if (!node->_label.empty()) {
        cout << prefix << "标签: " << node->_label << endl;
    }
    else {
        cout << prefix << "属性: " << node->_attribute << endl;
        for (const auto& child : node->_children) {
            cout << prefix << "--" << child.first << "--> ";
            printTree(child.second, prefix + "  ");
        }
    }
}

int main() {
    // 数据集
    vector<Sample> samples = {
        { {{"有其他选择?", "是"}, {"饿吗?", "是"}, {"价格", "$$$"}, {"餐馆类型", "法式"}, {"餐馆顾客人数", "有人"}, {"等待时间", "0-10"}}, "是" },
        { {{"有其他选择?", "是"}, {"饿吗?", "是"}, {"价格", "$"}, {"餐馆类型", "中餐"}, {"餐馆顾客人数", "客满"}, {"等待时间", "30-60"}}, "否" },
        { {{"有其他选择?", "否"}, {"饿吗?", "否"}, {"价格", "$"}, {"餐馆类型", "快餐"}, {"餐馆顾客人数", "有人"}, {"等待时间", "0-10"}}, "是" },
        { {{"有其他选择?", "是"}, {"饿吗?", "是"}, {"价格", "$"}, {"餐馆类型", "中餐"}, {"餐馆顾客人数", "客满"}, {"等待时间", "10-30"}}, "是" },
        { {{"有其他选择?", "是"}, {"饿吗?", "否"}, {"价格", "$$$"}, {"餐馆类型", "法式"}, {"餐馆顾客人数", "客满"}, {"等待时间", ">60"}}, "否" },
        { {{"有其他选择?", "否"}, {"饿吗?", "是"}, {"价格", "$$"}, {"餐馆类型", "意大利式"}, {"餐馆顾客人数", "有人"}, {"等待时间", "0-10"}}, "是" },
        { {{"有其他选择?", "否"}, {"饿吗?", "否"}, {"价格", "$"}, {"餐馆类型", "快餐"}, {"餐馆顾客人数", "无人"}, {"等待时间", "0-10"}}, "否" },
        { {{"有其他选择?", "否"}, {"饿吗?", "是"}, {"价格", "$$"}, {"餐馆类型", "中餐"}, {"餐馆顾客人数", "有人"}, {"等待时间", "0-10"}}, "是" },
        { {{"有其他选择?", "否"}, {"饿吗?", "否"}, {"价格", "$"}, {"餐馆类型", "快餐"}, {"餐馆顾客人数", "客满"}, {"等待时间", ">60"}}, "否" },
        { {{"有其他选择?", "是"}, {"饿吗?", "是"}, {"价格", "$$$"}, {"餐馆类型", "意大利式"}, {"餐馆顾客人数", "客满"}, {"等待时间", "10-30"}}, "否" },
        { {{"有其他选择?", "是"}, {"饿吗?", "否"}, {"价格", "$"}, {"餐馆类型", "中餐"}, {"餐馆顾客人数", "无人"}, {"等待时间", "0-10"}}, "否" },
        { {{"有其他选择?", "否"}, {"饿吗?", "是"}, {"价格", "$"}, {"餐馆类型", "快餐"}, {"餐馆顾客人数", "客满"}, {"等待时间", "30-60"}}, "是" }
    };
    set<string> _attributes = { "有其他选择?", "饿吗?", "价格", "餐馆类型", "餐馆顾客人数", "等待时间" };

    TreeNode* tree = buildTree(samples, _attributes);

    printTree(tree);
    delete tree;

    return 0;
}