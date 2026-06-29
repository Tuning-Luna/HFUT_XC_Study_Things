# encoding: utf-8
import os

# 排除目录
EXCLUDE_DIRS = {
    ".git",
    "docs",
    ".vscode",
    ".circleci",
    "site",
    ".github",
    "assets",
    "test-gh-pages",
    ".codegraph",
    ".claude",
    ".omo",
}

# README 文件名集合
README_MD = {"README.md", "readme.md", "index.md", "README.MD"}

# 二进制文件的 URL 前缀
BIN_URL_PREFIX = "https://github.com/Tuning-Luna/HFUT_XC_Study_Things/raw/main"


def list_files(course: str):
    filelist_texts = "## 文件列表\n\n"
    readme_path = ""

    for root, dirs, files in os.walk(course):
        dirs[:] = sorted(d for d in dirs if d not in EXCLUDE_DIRS)
        files.sort()

        level = root.replace(course, "").count(os.sep)

        # ⭐ 只在根目录寻找 README
        if level == 0:
            for f in files:
                if f in README_MD:
                    readme_path = os.path.join(root, f)

        indent = " " * 4 * level
        filelist_texts += f"{indent}- {os.path.basename(root)}\n"
        subindent = " " * 4 * (level + 1)

        for f in files:
            if f not in README_MD:
                rel_path = f"{root}/{f}".replace("\\", "/")
                filelist_texts += f"{subindent}- [{f}]({BIN_URL_PREFIX}/{rel_path})\n"

    return filelist_texts, readme_path


def generate_md(course: str, filelist_texts: str, readme_path: str):
    os.makedirs("docs", exist_ok=True)

    final_texts = ["\n\n", filelist_texts]
    if readme_path:
        with open(readme_path, "r", encoding="utf-8") as file:
            final_texts = file.readlines() + final_texts
    out_path = os.path.join("docs", f"{course}.md")
    with open(out_path, "w", encoding="utf-8") as file:
        file.writelines(final_texts)
    print(f"  ✓ {course}")


if __name__ == "__main__":
    courses = sorted(d for d in os.listdir(".") if os.path.isdir(d) and d not in EXCLUDE_DIRS)

    if not courses:
        print("没有发现课程目录（排除 EXCLUDE_DIRS 后）。请在仓库根目录运行脚本。")

    print(f"发现 {len(courses)} 个课程目录，开始生成文档...")
    for course in courses:
        filelist_texts, readme_path = list_files(course)
        generate_md(course, filelist_texts, readme_path)
    print("全部生成完成！")
