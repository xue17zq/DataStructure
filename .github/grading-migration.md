# 迟交计分迁移记录

- 课程：**数据结构**（`data-structure`）
- 制度生效日期：2026-11-10
- 共享审核器：course-pr-reviewer v0.15.0（`27337bbae625c39ce40489c06bb48f13d53d16e9`）
- 成绩分支：`grades`（初始提交 `c7ee4212df79761185fb1a3d9ec4c9d9831fd379`，孤儿分支，由审核器写入）
- 配置文件：`.github/course-review.yml`

## 1. 已发布作业（冻结为 legacy）

以下清单按各仓库发布历史核对，**不按 deadline 是否已经过去推测**。这些作业不实行分段迟交扣分，
内容合格即自动合并，收据里 `score_cap_percent` 与 `awarded_points` 均为 `null`。

| 作业 | 截止时间 | 模式 |
|---|---|---|
| Lab1 | 2026-09-08T00:00:00+08:00 | legacy |
| Lab2 | 2026-09-11T00:00:00+08:00 | legacy |
| Lab3 | 2026-09-15T00:00:00+08:00 | legacy |
| Lab4 | 2026-09-18T00:00:00+08:00 | legacy |
| Lab5 | 2026-09-22T00:00:00+08:00 | legacy |
| Lab6 | 2026-09-29T00:00:00+08:00 | legacy |
| Lab7 | 2026-10-02T00:00:00+08:00 | legacy |
| Lab8 | 2026-10-11T00:00:00+08:00 | legacy |
| Lab9 | 2026-10-16T00:00:00+08:00 | legacy |
| Lab10 | 2026-10-22T00:00:00+08:00 | legacy |
| Lab11 | 2026-10-23T00:00:00+08:00 | legacy |
| Lab12 | 2026-10-27T00:00:00+08:00 | legacy |
| Lab13 | 2026-10-29T00:00:00+08:00 | legacy |
| Lab14 | 2026-10-30T00:00:00+08:00 | legacy |
| Lab15 | 2026-11-03T00:00:00+08:00 | legacy |
| Lab16 | 2026-11-07T00:00:00+08:00 | legacy |
| Lab17 | 2026-11-13T00:00:00+08:00 | legacy |

## 2. 首个 tiered 作业

**Lab18（查找）** 是第一个启用分段迟交计分的作业。

| 作业 | 截止时间 | 模式 | 满分 |
|---|---|---|---|
| Lab18 | 2026-11-20T00:00:00+08:00 | tiered | 10.00 |

配置：

```yaml
  Lab18:
    enabled: true
    deadline: "2026-11-20T00:00:00+08:00"
    grading:
      mode: tiered
      max_points: "10.00"
```

分档沿用 `grading.defaults.tiered.bands`：按时 100%、48 小时内 90%、7 天内 75%、
14 天内 60%、28 天内 40%、更晚 20%；截止前有实质提交且在 48 小时内修正通过的按 100%。

发布前先运行：

```bash
course-pr-reviewer grade validate --config .github/course-review.yml --students .github/students.yml
```

## 3. 旧 PR 如何处理

- **已合并的历史 PR**：不回填成绩，不补算分段扣分，不修改历史评论。
- **仍打开的旧 PR**：下一次推送时按新规则审核；内容合格即合并，`legacy` 作业不扣分。
- **已关闭的旧 PR**：不自动重新打开，学生可重新提交合规 PR。
- 本迁移没有批量关闭、批量重开或批量修改任何学生 PR。

## 4. 上线顺序（必须遵守）

1. 先把 `LiuXiYing/course-pr-reviewer` 的新版本推到 `main`；
2. 再推送本仓库的配置与工作流。

若顺序反了，旧审核器读不到 `grading` 字段会判定配置无效，所有 PR 会停在 `ERROR`（不合并、不计分）。
此时把审核器版本推上去即可恢复，不需要学生重新提交。

## 5. 回滚

1. 把 `.github/workflows/` 里 `LiuXiYing/course-pr-reviewer@...` 改回上一版本 commit；
2. 从 `.github/course-review.yml` 删除 `grading` 块与 `features.grading`，恢复 `close_late_pr: true`，
   并删除每个作业的 `grading:` 两行；
3. `grades` 分支保留只读即可，不影响审核；确认不再使用后再删除；
4. 已写入的 `receipts/` 不要手工修改，保留作为审计记录。

## 6. 已知限制

- 公开仓库的 `grades` 分支同样是**公开**的，其中只有学号、GitHub 账号和分数，没有姓名与申诉材料。
- 48 小时修正期依赖截止前的实质性提交快照；无法核验时按真实迟交档计分，并在评论中提示可联系教师复核。
- 教师手工改分走 `overrides/`（schema 已定，尚无表单界面），不改写原始收据。
