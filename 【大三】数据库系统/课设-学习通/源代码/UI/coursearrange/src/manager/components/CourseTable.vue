<template>
<div>
  <div class="class-table">
    <div class="top-select">
      <el-select v-model="value1" placeholder="学期">
        <el-option
          v-for="item in semester"
          :key="item.value"
          :label="item.label"
          :value="item.value"
        ></el-option>
      </el-select>
      <el-select v-model="value2" placeholder="年级" @change="queryClass">
        <el-option v-for="item in grade" :key="item.value" :label="item.label" :value="item.value"></el-option>
      </el-select>
      <el-select v-model="value3" placeholder="班级" @change="queryCoursePlan">
      
        <el-option
          v-for="item in classNo"
          :key="item.value"
          :label="item.label"
          :value="item.value"
        ></el-option>
      </el-select>
    </div>

   <!-- 添加课程计划 -->
        <div style="margin:10px;" v-if="!isTeacher">
            <el-button class="add-button" size="small" type="primary" @click="addClassTask1()">手动添加</el-button>
 
            <el-button class="add-button" size="small" type="primary" @click="deleteClassTask()">
              手动删除
            </el-button>
        </div>
      
    <div class="table-wrapper">
      <div class="tabel-container">
        <table>
          <thead>
            <tr>
              <th>时间</th>
              <th
                v-for="(weekNum, weekIndex) in classTableData.courses.length"
                :key="weekIndex"
              >{{'周' + digital2Chinese(weekIndex + 1, 'week')}}</th>
            </tr>
          </thead>
          <tbody>
            <tr v-for="(lesson, lessonIndex) in classTableData.lessons" :key="lessonIndex">
              <td>
                <p>{{'第' + digital2Chinese(lessonIndex+1) + "节"}}</p>
                <p class="period">{{ lesson }}</p>
              </td>

              <td
                v-for="(course, courseIndex) in classTableData.courses"
                :key="courseIndex"
              >{{classTableData.courses[courseIndex][lessonIndex] || '-'}}</td>
            </tr>
          </tbody>
        </table>
      </div>
    </div>

    
  </div>

      <!-- 弹出表单添加 -->
    <el-dialog title="添加课程(仅在空余时间可以添加)" :visible.sync="visible">
      <el-form :model="addClassTask1Form" label-position="left" label-width="80px" :rules="addClassTask1Rules">
        <el-form-item label="学期" prop="semester">
          <el-input v-model="addClassTask1Form.semester" autocomplete="off"></el-input>
        </el-form-item>
        <el-form-item label="年级" prop="gradeNo">
          <el-input v-model="addClassTask1Form.gradeNo" autocomplete="off"></el-input>
        </el-form-item>
        <el-form-item label="班级编号" prop="classNo">
          <el-input v-model="addClassTask1Form.classNo" autocomplete="off"></el-input>
        </el-form-item>
        <!-- <el-form-item label="课程编号" prop="courseNo">
          <el-input v-model="addClassTask1Form.courseNo" autocomplete="off"></el-input>
        </el-form-item> -->
        <el-form-item label="课程名" prop="courseName">
          <el-input v-model="addClassTask1Form.courseName" autocomplete="off"></el-input>
        </el-form-item>
        <!-- <el-form-item label="讲师编号" prop="teacherNo">
          <el-input v-model="addClassTask1Form.teacherNo" autocomplete="off"></el-input>
        </el-form-item> -->
        <el-form-item label="讲师名字" prop="realname">
          <el-input v-model="addClassTask1Form.realname" autocomplete="off"></el-input>
        </el-form-item>
        <!-- <el-form-item label="课程属性" prop="courseAttr">
          <el-input v-model="addClassTask1Form.courseAttr" autocomplete="off"></el-input>
        </el-form-item>
        <el-form-item label="学生人数" prop="studentNum">
          <el-input v-model="addClassTask1Form.studentNum" autocomplete="off"></el-input>
        </el-form-item>
        <el-form-item label="周学时" prop="weeksNumber">
          <el-input v-model="addClassTask1Form.weeksNumber" autocomplete="off"></el-input>
        </el-form-item>
        <el-form-item label="周数" prop="weeksSum">
          <el-input v-model="addClassTask1Form.weeksSum" autocomplete="off"></el-input>
        </el-form-item>
        <el-form-item label="是否固定" prop="isFix">
          <el-input v-model="addClassTask1Form.isFix" autocomplete="off"></el-input>
        </el-form-item> -->
        <!-- <el-form-item label="上课时间" prop="classTime">
          <el-input v-model="addClassTask1Form.classTime" autocomplete="off"></el-input>
        </el-form-item> -->
        <el-select v-model="addClassTask1Form.classTime" placeholder="上课时间">
          <el-option
          v-for="item in classTime"
          :key="item.value"
          :label="item.label"
          :value="item.value"
          ></el-option>
         </el-select>
      </el-form>
      <div slot="footer" class="dialog-footer">
        <el-button @click="visible = false">取 消</el-button>
        <el-button type="primary" @click="commit()">提 交</el-button>
      </div>
    </el-dialog>


    <!-- 弹出表单添加 -->
    <el-dialog title="删除课程(仅在有课时间可以删除)" :visible.sync="visible1">
      <el-form :model="deleteClassTaskForm" label-position="left" label-width="80px" :rules="deleteClassTaskRules">
        
        <el-form-item label="学期" prop="semester">
          <el-input v-model="deleteClassTaskForm.semester" autocomplete="off"></el-input>
        </el-form-item>
        <!-- <el-form-item label="年级" prop="gradeNo">
          <el-input v-model="deleteClassTaskForm.gradeNo" autocomplete="off"></el-input>
        </el-form-item> -->
        <el-form-item label="班级编号" prop="classNo">
          <el-input v-model="deleteClassTaskForm.classNo" autocomplete="off"></el-input>
        </el-form-item>
        <!-- <el-form-item label="课程名" prop="courseName">
          <el-input v-model="deleteClassTaskForm.courseName" autocomplete="off"></el-input>
        </el-form-item>
        <el-form-item label="讲师名字" prop="realname">
          <el-input v-model="deleteClassTaskForm.realname" autocomplete="off"></el-input>
        </el-form-item> -->
         <el-select v-model="deleteClassTaskForm.classTime" placeholder="上课时间">
          <el-option
          v-for="item in classTime"
          :key="item.value"
          :label="item.label"
          :value="item.value"
          ></el-option>
         </el-select>
      </el-form>
      <div slot="footer" class="dialog-footer">
        <el-button @click="visible = false">取 消</el-button>
        <el-button type="primary" @click="delete_commit()">提 交</el-button>
      </div>
    </el-dialog>



</div>
</template>




<script>
export default {
  data() {
    return {
      importBtnDisabled: false, // 按钮是否禁用,
      loading: false,
      classTaskData: [],
      semesterData: [],
      addClassTask1Form: {
        semester: '',
        gradeNo: '',
        classNo: '',
        // courseNo: '',
        courseName: '',
        // teacherNo: '',
        realname: '',
        // courseAttr: '',
        // studentNum: '',
        // weeksNumber: '',
        // weeksSum: '',
        // isFix: '',
        classTime: ''
      },
      visible: false,
      page: 1,
      total: 0,
      pageSize: 10,
      // 学期选择绑定的值
      value: "2021-2022-1",
      // 当前选择的学期
      semester: "2021-2022-1",
      fileList: [],
      addClassTask1Rules: {
        semester: [{ required: true, message: '请输入学期', trigger: 'blur' }],
        gradeNo: [{ required: true, message: '请输入年级编号', trigger: 'blur' }],
        classNo: [{ required: true, message: '请输入班级编号', trigger: 'blur' }],
        // courseNo: [{ required: true, message: '请输入课程编号', trigger: 'blur' }],
        courseName: [{ required: true, message: '请输入课程名称', trigger: 'blur' }],
        // teacherNo: [{ required: true, message: '请输入讲师编号', trigger: 'blur' }],
        realname: [{ required: true, message: '请输入讲师姓名', trigger: 'blur' }],
        // courseAttr: [{ required: true, message: '请输入课程属性', trigger: 'blur' }],
        // studentNum: [{ required: true, message: '请输入班级学生人数', trigger: 'blur' }],
        // weeksNumber: [{ required: true, message: '请输入周学时', trigger: 'blur' }],
        // weeksSum: [{ required: true, message: '请输入上课周数', trigger: 'blur' }],
        // isFix: [{ required: true, message: '是否固定上课时间', trigger: 'blur' }],
        classTime: [{ required: true, message: '请选择时间', trigger: 'blur' }]
      },
      
      importBtnDisabled: false, // 按钮是否禁用,
      loading: false,
      classTaskData: [],
      semesterData: [],
      deleteClassTaskForm: {
        semester: '',
        gradeNo: '',
        classNo: '',
        // courseNo: '',
        courseName: '',
        // teacherNo: '',
        realname: '',
        // courseAttr: '',
        // studentNum: '',
        // weeksNumber: '',
        // weeksSum: '',
        // isFix: '',
        classTime: ''
      },
      visible1: false,
      page: 1,
      total: 0,
      pageSize: 10,
      // 学期选择绑定的值
      value: "2021-2022-1",
      // 当前选择的学期
      semester: "2021-2022-1",
      fileList: [],
      deleteClassTaskRules: {
        semester: [{ required: true, message: '请输入学期', trigger: 'blur' }],
        // gradeNo: [{ required: true, message: '请输入年级编号', trigger: 'blur' }],
        classNo: [{ required: true, message: '请输入班级编号', trigger: 'blur' }],
        // courseNo: [{ required: true, message: '请输入课程编号', trigger: 'blur' }],

        // courseName: [{ required: true, message: '请输入课程名称', trigger: 'blur' }],

        // teacherNo: [{ required: true, message: '请输入讲师编号', trigger: 'blur' }],

        // realname: [{ required: true, message: '请输入讲师姓名', trigger: 'blur' }],

        // courseAttr: [{ required: true, message: '请输入课程属性', trigger: 'blur' }],
        // studentNum: [{ required: true, message: '请输入班级学生人数', trigger: 'blur' }],
        // weeksNumber: [{ required: true, message: '请输入周学时', trigger: 'blur' }],
        // weeksSum: [{ required: true, message: '请输入上课周数', trigger: 'blur' }],
        // isFix: [{ required: true, message: '是否固定上课时间', trigger: 'blur' }],
        classTime: [{ required: true, message: '请选择时间', trigger: 'blur' }]
      },


      //
      classTime:[
        {
          value:"01",
          label:"周1第1节"
        },
        {
          value:"02",
          label:"周1第2节"
        },
        {
          value:"03",
          label:"周1第3节"
        },
        {
          value:"04",
          label:"周1第4节"
        },
        {
          value:"05",
          label:"周1第5节"
        },
        {
          value:"06",
          label:"周2第1节"
        },
        {
          value:"07",
          label:"周2第2节"
        },
        {
          value:"08",
          label:"周2第3节"
        },
        {
          value:"09",
          label:"周2第4节"
        },
        {
          value:"10",
          label:"周2第5节"
        },
        {
          value:"11",
          label:"周3第1节"
        },
        {
          value:"12",
          label:"周3第2节"
        },
        {
          value:"13",
          label:"周3第3节"
        },
        {
          value:"14",
          label:"周3第4节"
        },
        {
          value:"15",
          label:"周3第5节"
        },
        {
          value:"16",
          label:"周4第1节"
        },
        {
          value:"17",
          label:"周4第2节"
        },
        {
          value:"18",
          label:"周4第3节"
        },
        {
          value:"19",
          label:"周4第4节"
        },
        {
          value:"20",
          label:"周4第5节"
        },
        {
          value:"21",
          label:"周5第1节"
        },
        {
          value:"22",
          label:"周5第2节"
        },
        {
          value:"23",
          label:"周5第3节"
        },
        {
          value:"24",
          label:"周5第4节"
        },
        {
          value:"25",
          label:"周5第5节"
        },
        {
          value:"26",
          label:"周6第1节"
        },
        {
          value:"27",
          label:"周6第2节"
        },
        {
          value:"28",
          label:"周6第3节"
        },
        {
          value:"29",
          label:"周6第4节"
        },
        {
          value:"30",
          label:"周6第5节"
        },
      ],

      // 查询课表
      semester: [
        {
          value: "2021-2022-1",
          label: "2021-2022-1"
        }
      ],
      grade: [
        {
          value: "01",
          label: "高一"
        },
        {
          value: "02",
          label: "高二"
        },
        {
          value: "03",
          label: "高三"
        }
      ],
      classNo: [
        {
          value: "",
          label: ""
        }
      ],
      value1: "",
      value2: "",
      value3: "",
      classTableData: {
        lessons: [
          "07.20-8.55",
          "9.10-10.45",
          "11.00-12.35",
          "14.20-15.55",
          "16.10-17.45"

        ],
        courses: [[],[],[],[],[],[]

        ]
      }
    };
  },
  created() {

  },

 /**
   * 加载Vue实例时执行
   */
  mounted() {
    setInterval(() => {
      this.getTime();
    }, 1000);
    
    let admin = window.localStorage.getItem('admin')
    if(admin != null){
      this.name = (JSON.parse(admin)).realname
    } else {
      let teacher = window.localStorage.getItem('teacher')
      if (teacher != null) {
        this.name = (JSON.parse(teacher)).realname
      }
    }
  },

  computed: {
    isTeacher:()=>{
       console.log(window.localStorage.getItem('admin'));
      console.log(window.localStorage.getItem('teacher'));
      return window.localStorage.getItem('teacher') != null;
    }
  },

  methods: {

    // 获取系统时间
    getTime() {
      this.time = new Date().toLocaleString();
    },

    
    // 查询班级编号，班级名
    queryClass() {
      this.$axios
        .get("http://localhost:8080/class-grade/" + this.value2)
        .then(res => {
          //alert(this.value2)
          let r = res.data.data;
          this.classNo.splice(0,this.classNo.length); 
          this.value3 = ''
          r.map(v=>{
            this.classNo.push({
              value:v.classNo,
              lable:v.className
            })
          })
        })
        .catch(error => {
          this.$message.error("失败")
        });
    },

    // 查询课程表
    queryCoursePlan() {
      this.classTableData.courses.map((item, index)=>{
        this.classTableData.courses[index].splice(0,this.classTableData.courses[index].length)
      })
      this.$axios
        .get("http://localhost:8080/courseplan/" + this.value3)
        .then(res => {
          console.log(res)
          let courseData = res.data.data;
          let level = 0;
          let times = 0;
          for (let index = 0; index < courseData.length; index++) {
            times = times + 1;
            const item = courseData[index];
            if(parseInt(item.classTime) != times){
              this.classTableData.courses[level].push("");
              index = index - 1;
            }
            else{
              
              this.classTableData.courses[level].push(item.teacher.realname + "-" + item.courseInfo.courseName + "(" + item.classroomNo + ")");
            }
            if((times % 5) == 0){
              level = level + 1;
            }
          }
          this.$message({message:'查询成功', type: 'success'})
        })
    },

    /**
     * 数字转中文
     * @param {Number} num 需要转换的数字
     * @param {String} identifier 标识符
     * @returns {String} 转换后的中文
     */
    digital2Chinese(num, identifier) {
      const character = [
        "零",
        "一",
        "二",
        "三",
        "四",
        "五",
        "六",
        // "七",
        // "八",
      ];
      return identifier === "week" && (num === 0 || num === 7)
        ? "日"
        : character[num];
    },

    

    // 提交添加
    commit() {
      this.$axios.post("http://localhost:8080/addcourseplan", this.addClassTask1Form)
      .then(res => {
        if (res.data.code == 0) {
          // 添加完成
          this.visible = false
          this.$message({message: "手动添加课程任务成功！", type: "success"})
          

          //更新查询
          this.classTableData.courses.map((item, index)=>{
        this.classTableData.courses[index].splice(0,this.classTableData.courses[index].length)
      })
      this.$axios
        .get("http://localhost:8080/courseplan/" + this.value3)
        .then(res => {
          console.log(res)
          let courseData = res.data.data;
          let level = 0;
          let times = 0;
          for (let index = 0; index < courseData.length; index++) {
            times = times + 1;
            const item = courseData[index];
            if(parseInt(item.classTime) != times){
              this.classTableData.courses[level].push("");
              index = index - 1;
            }
            else{
              
              this.classTableData.courses[level].push(item.teacher.realname + "-" + item.courseInfo.courseName + "(" + item.classroomNo + ")");
            }
            if((times % 5) == 0){
              level = level + 1;
            }
          }
          this.$message({message:'查询成功', type: 'success'})
        })




        } else {
          alert(res.data.message)
        }
      })
      .catch(error => {

      })
    },


    delete_commit() {
      console.log(this.deleteClassTaskForm)
      this.$axios.post("http://localhost:8080/deletecourseplan", this.deleteClassTaskForm)
      .then(res => {
        if (res.data.code == 0) {
          // 添加完成
          this.visible1 = false
          this.$message({message: "手动删除课程任务成功！", type: "success"})

          //更新查询
          this.classTableData.courses.map((item, index)=>{
        this.classTableData.courses[index].splice(0,this.classTableData.courses[index].length)
      })
      this.$axios
        .get("http://localhost:8080/courseplan/" + this.value3)
        .then(res => {
          console.log(res)
          let courseData = res.data.data;
          let level = 0;
          let times = 0;
          for (let index = 0; index < courseData.length; index++) {
            times = times + 1;
            const item = courseData[index];
            if(parseInt(item.classTime) != times){
              this.classTableData.courses[level].push("");
              index = index - 1;
            }
            else{
              
              this.classTableData.courses[level].push(item.teacher.realname + "-" + item.courseInfo.courseName + "(" + item.classroomNo + ")");
            }
            if((times % 5) == 0){
              level = level + 1;
            }
          }
          this.$message({message:'查询成功', type: 'success'})
        })
        } else {
          alert(res.data.message)
        }
      })
      .catch(error => {

      })
    },

    // 手动添加课程任务
    addClassTask1() {
      this.visible = true
    },


    // 下载模板
    deleteClassTask() {
      this.visible1 = true
    },

    // 得到对应选中的年级
    handleSelectChange(val) {
      // 这里的V就是选择的学期了
      this.semester = val
      
    },



  }


};
</script>


<style lang="less" scoped>


.class-table {
  .top-select {
    text-align: left;
    margin-left: 7px;
  }
  .table-wrapper {
    width: 100%;
    height: 100%;
    overflow: auto;
  }
  .tabel-container {
    margin: 7px;
    font-size: 30px;
    table {
      table-layout: fixed;
      width: 100%;
      word-wrap:break-word;
      word-break:break-all;
      border-collapse:collapse;
      thead {
        background-color: #67a1ff;
        th {
          color: #fff;
          line-height: 17px;
          font-weight: normal;
          font-size: 15px;
        }
      }
      tbody {
        background-color: #eaf2ff;
        td {
          font-size: 13px;
          color: #677998;
          line-height: 13px;
        }
      }
      th,
      td {
        width: 60px;
        padding: 12px 2px;
        font-size: 12px;
        text-align: center;
      }

      tr td:first-child {
        color: #333;
        .period {
          font-size: 12px;
        }
      }
    }
  }
}
</style>