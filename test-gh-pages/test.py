# 此文件只是测试递归生成当前目录文件夹并存放在 docs 目录下的脚本
# 在项目中不会使用，github workflow 中使用的是 gen-md.py 脚本

# encoding: utf-8
from pathlib import Path
from typing import Tuple, Optional
import os

EXCLUDE_DIRS = {'.git', 'docs', '.vscode', '.circleci', 'site', '.github'}
README_MD = {'README.md', 'readme.md', 'index.md'}
BIN_URL_PREFIX = 'https://github.com/Tuning-Luna/HFUT_XC_Study_Things/raw/main'


def list_files(course: Path) -> Tuple[str, Optional[Path]]:
    """
    遍历 course 目录，生成 Markdown 的文件列表文本并返回可能存在的 README 路径。
    返回 (filelist_texts, readme_path_or_None)
    """
    filelist_texts = '## 文件列表\n\n'
    readme_path: Optional[Path] = None

    # 使用 rglob / walk 效果类似，但保持原脚本的树形缩进逻辑
    for root, dirs, files in os.walk(course):
        # 过滤掉不需要进入的目录（原地修改 dirs，避免进入这些子目录）
        dirs[:] = [d for d in dirs if d not in EXCLUDE_DIRS]

        level = Path(root).relative_to(course).parts.__len__() if Path(root) != course else 0
        indent = ' ' * 4 * level
        filelist_texts += f'{indent}- {Path(root).name}\n'
        subindent = ' ' * 4 * (level + 1)
        # 按文件名排序以保证输出稳定
        for f in sorted(files):
            if f in README_MD:
                readme_path = Path(root) / f
            else:
                # 生成相对仓库的 unix 风格路径用于链接
                rel_path = Path(root, f).as_posix()
                filelist_texts += f'{subindent}- [{f}]({BIN_URL_PREFIX}/{rel_path})\n'

    return filelist_texts, readme_path


def generate_md(course: Path, filelist_texts: str, readme_path: Optional[Path]) -> Path:
    """
    将 README（如果有）+ 文件列表 写入 docs/{course.name}.md
    返回写入的文件路径
    """
    docs_dir = Path('docs')
    docs_dir.mkdir(parents=True, exist_ok=True)  # ✅ 确保 docs 存在

    final_texts = ['\n\n', filelist_texts]
    if readme_path and readme_path.exists():
        # 读取 README 内容并放在最前面
        final_texts = readme_path.read_text(encoding='utf-8').splitlines(keepends=True) + final_texts

    out_path = docs_dir / f'{course.name}.md'
    out_path.write_text(''.join(final_texts), encoding='utf-8')
    return out_path


if __name__ == '__main__':
    # 当前工作目录下的一级目录视为 “课程” 目录（排除 EXCLUDE_DIRS）
    cwd = Path('.')
    courses = [d for d in sorted(cwd.iterdir()) if d.is_dir() and d.name not in EXCLUDE_DIRS]

    if not courses:
        print('没有发现课程目录（排除 EXCLUDE_DIRS 后）。请在仓库根目录运行脚本。')

    for course in courses:
        filelist_texts, readme_path = list_files(course)
        out_file = generate_md(course, filelist_texts, readme_path)
        print(f'生成: {out_file}')
