import express from "express"

const app = express()

app.get("/", (req, res) => {
  const q = req.query.q || ""

  // ❌ 漏洞点：直接拼接
  res.send(`
    <h1>Search</h1>
    <div>Result: ${q}</div>
  `)
})

app.listen(3000, () => {
  console.log("http://localhost:3000/")
})
// http://localhost:3000/?q=<script>alert('XSS')</script>
