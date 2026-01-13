// 参数区
base_w = 80;
base_d = 100;
base_h = 6;

back_h = 80;
back_t = 6;
back_angle = 70;

front_h = 6;   // 前挡板高度
front_t = 4;   // 前挡板厚度

slot_w = 12;
slot_d = 10;


$fn = 64;

// 底座
module base_plate() {
    difference() {
        cube([base_w, base_d, base_h]);

        // 手机卡槽（可忽略，也可用作装饰）
        translate([base_w/2 - slot_w/2, base_d - slot_d - 5, -1])
            cube([slot_w, slot_d, base_h + 2]);
    }
}

// 支撑背板
module back_support() {
    rotate([-back_angle, 0, 0])
        cube([base_w, back_t, back_h]);
}

// 前挡板
module front_stop() {
    translate([0, 0, 0])
        cube([base_w, front_t, front_h]);
}

// 底座支撑脚
module foot(pos_x, pos_y) {
    translate([pos_x, pos_y, -2])
        cube([6, 6, 2]);
}

// 主体组合
module phone_stand() {
    union() {
        base_plate();
        front_stop();

        translate([0, base_h, 0])
            back_support();

        // 底座四角脚
        foot(0, 0);
        foot(base_w - 10, 0);
        foot(0, base_d - 10);
        foot(base_w - 10, base_d - 10);
    }
}

// 渲染
phone_stand();
