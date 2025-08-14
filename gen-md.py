# encoding: utf-8
import os

EXCLUDE_DIRS = {'.git', 'docs', '.vscode', '.circleci', 'site', '.github','assets','test-gh-pages'}
README_MD = {'README.md', 'readme.md', 'index.md'}

BIN_URL_PREFIX = 'https://github.com/Tuning-Luna/HFUT_XC_Study_Things/raw/main'

def list_files(course: str):
    filelist_texts = '## 文件列表\n\n'
    readme_path = ''
    for root, dirs, files in os.walk(course):
        # 在递归中排除目录
        dirs[:] = [d for d in dirs if d not in EXCLUDE_DIRS]

        level = root.replace(course, '').count(os.sep)
        indent = ' ' * 4 * level
        filelist_texts += f'{indent}- {os.path.basename(root)}\n'
        subindent = ' ' * 4 * (level + 1)
        for f in files:
            if f in README_MD:
                readme_path = os.path.join(root, f)
            else:
                rel_path = f'{root}/{f}'.replace('\\', '/')
                filelist_texts += f'{subindent}- [{f}]({BIN_URL_PREFIX}/{rel_path})\n'
    return filelist_texts, readme_path

def generate_md(course: str, filelist_texts: str, readme_path: str):
    final_texts = ['\n\n', filelist_texts]
    if readme_path:
        with open(readme_path, 'r', encoding='utf-8') as file:
            final_texts = file.readlines() + final_texts
    out_path = os.path.join('docs', f'{course}.md')
    with open(out_path, 'w', encoding='utf-8') as file:
        file.writelines(final_texts)

if __name__ == '__main__':
    courses = [
        d for d in os.listdir('.')
        if os.path.isdir(d) and d not in EXCLUDE_DIRS
    ]
    for course in courses:
        filelist_texts, readme_path = list_files(course)
        generate_md(course, filelist_texts, readme_path)
