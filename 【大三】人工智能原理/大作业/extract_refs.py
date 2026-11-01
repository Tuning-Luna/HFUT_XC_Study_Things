from pypdf import PdfReader
import re


def extract_references(pdf_path):
    reader = PdfReader(pdf_path)
    full_text = ""
    for page in reader.pages:
        full_text += page.extract_text() + "\n\n"

    # 查找参考文献部分
    # 参考文献通常在 "REFERENCES" 或 "References" 之后
    ref_pattern = r"(?i)(?:REFERENCES|References|Bibliography)\s*\n"
    match = re.search(ref_pattern, full_text)

    if match:
        ref_start = match.end()
        references = full_text[ref_start:]
    else:
        # 如果没有找到标题，尝试从最后几页提取
        # 参考文献通常在最后10-20页
        total_pages = len(reader.pages)
        start_page = max(0, total_pages - 20)
        references = ""
        for i in range(start_page, total_pages):
            references += reader.pages[i].extract_text() + "\n\n"

    # 保存到文件
    with open("references.txt", "w", encoding="utf-8") as f:
        f.write(references)

    print(f"Extracted references (length: {len(references)} characters)")
    return references


if __name__ == "__main__":
    refs = extract_references("2402.06196v3.pdf")
    # 打印前2000个字符
    print(refs[:2000])
