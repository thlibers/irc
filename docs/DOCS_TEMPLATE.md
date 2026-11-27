<!---
- Every <h1> should be the only block in uppercase and centered using align="center" (see below)
- The first <h1> block is always the name of the documents
- Under the first <h1> block do a short description of what you will explain later on the documents
- Under the short description put a <h1> who will contain the summary of the page
- You are free to use the number of section (by section i talk about <h1>/<h2>/<h3>/...) that you want.
- If you use a schema or import a picture into the documentation, put a simple <p> with a short caption under it, so the reader knows what it shows
- The maximum depth you will use for sub section will be <h3>, ddont use <h4> or highers.
- Fell also free to use > [!WARNING], > [!NOTE], ...
- The last section should be a glossary section who will explain every technical notion. to indicate to the reader that a glossary exist for that word place a <sup> tag who surrend the index associate with that notion (e.g IRC<sup>1</sup>). The glossary should be short and understandable easly for someone who doesnt know anything about Comp. sci.
- Each glossary entry is a single line in the format Notion<sup>index</sup>: short definition (no table)
--->

<div align="center">
	<h1>DOCS_TEMPLATE</h1>
</div>

This documents is not part of the documentation and will only be used to unified documentation format across all documents

<div align="center">
	<h1>SUMMARY</h1>
</div>

- [IDK](#idk) <!-- Link to the section, anchor = title lowercased, words joined by '-' -->
	- [IDK SUBSECTION1](#idk-subsection1)
		- [IDK SUBSUBSECTION1](#idk-subsubsection1)
		- [IDK SUBSUBSECTION2](#idk-subsubsection2)
	- [IDK SUBSECTION2](#idk-subsection2)
- [IDK2](#idk2)
- [GLOSSARY](#glossary)

<div align="center">
	<h1>IDK</h1>
</div>

This is the body of the first main section. A section title is written in **UPPER CASE** so it is easy to spot while scrolling. GitHub generates the anchor<sup>1</sup> of a section by lowercasing its title and joining the words with `-` (e.g. `IDK SUBSECTION1` becomes `#idk-subsection1`), which is what the SUMMARY links point to.

One section explains **one thing** (a class, a mechanism, a step of the program). If a section has to be cut into smaller parts, use a subsection.

<div align="center">
	<h2>IDK SUBSECTION1</h2>
</div>

A subsection uses a `<h2>` and is named `<SECTION> <SUBSECTION>`. It goes one level deeper in the SUMMARY than its parent section.

<div align="center">
	<h3>IDK SUBSUBSECTION1</h3>
</div>

A sub-subsection uses a `<h3>` and is named `<SECTION> <SUBSECTION> <SUBSUBSECTION>`. It goes one more level deeper in the SUMMARY.

<div align="center">
	<h3>IDK SUBSUBSECTION2</h3>
</div>

A second sub-subsection of `IDK SUBSECTION1`: sub-subsections of the same subsection are grouped together in the SUMMARY.

<div align="center">
	<h2>IDK SUBSECTION2</h2>
</div>

A subsection without children does not get any nested entry in the SUMMARY: `IDK SUBSECTION2` above is a leaf, so nothing is listed under it.

<div align="center">
	<h1>IDK2</h1>
</div>

A second main section. It works exactly like `IDK`, and is separated from the previous section by a blank line.

If your explanation needs a **schema or a picture**, import it and put a simple `<p>` with a short caption under it, so the reader knows what they are looking at:

<p align="center">
	<img src="schemadocs.svg" alt="example schema"/>
</p>
<p align="center"><i>Figure 1: a one line caption explaining what the schema shows</i></p>

The caption is not mandatory when the surrounding text already explains the schema.

<div align="center">
	<h1>GLOSSARY</h1>
</div>

Each entry is a single line in the format `Notion<sup>index</sup>: short definition`:

<!-- An entry stays short: one or two lines maximum. If an explanation needs more, it probably deserves its own section in the body. -->
- anchor<sup>1</sup>: The invisible target a link points to. On this page, the SUMMARY links jump to section anchors.
- poll()<sup>2</sup>: A function that asks the system "tell me which client has something to say", so the server can sleep instead of checking every client in a loop.
