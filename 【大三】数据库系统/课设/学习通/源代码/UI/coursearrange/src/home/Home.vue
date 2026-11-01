<template>
  <div class="wrapper" :style="wrapper">
    <div>
      <h1 class="title">Welcome To 中小学智能排课系统</h1>
      <div class="inner">
        <h3><router-link to="admin/login" style="text-decoration:none">管理员讲师入口</router-link></h3>
        <h3><router-link to="student/login" style="text-decoration:none;">学生入口</router-link></h3>
      </div>
    </div>
    
    <h3 class="footer"><router-link to="/" style="text-decoration:none;">作者：靳凯程、陈少凯</router-link></h3>
    <h3>Copy Right：© 2022 计算机19-2班数据库课程设计</h3>
    
  </div>
</template>

<script>
export default {
  name: 'Home',
  data() {
    return {
      wrapper: {
         backgroundImage: "url(" + require("../images/image1.jpg") + ")",
            // backgroundRepeat: "no-repeat",
            
      },
    }
  }
}
</script>

<style lang="less" scoped>
  .footer {
    top: 66%;
    margin-bottom: 34%;
    position: relative;
    left: 0%;
  }

  .wrapper {
    background-image: "@images/psb.jpg";
    height: 100%;
    width: 100%;
    position: absolute;
  }

  .title {
    text-align: left;
    margin-left: 35%;
    margin-top: 40px;
  }
  .inner {
    text-align: center;
    position: absolute;
    top: 300px;
    left: 45%;
  }
</style>