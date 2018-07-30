import sys
import bpy


def main(file_path, export_script):
  # switch to the working_scene
  bpy.context.screen.scene = bpy.data.scenes['working_scene']

  # switch to edit mode
  # bpy.ops.object.mode_set(mode='OBJECT')

  text = bpy.data.texts.load(export_script)

  # insert the export script text into text edit panel and run it as a script
  ctx = bpy.context.copy()
  ctx['edit_text'] = text
  bpy.ops.text.run_script(ctx)
  
  bpy.ops.io.export_mesh(filepath=file_path)


if __name__ == '__main__':
  main(sys.argv[-2], sys.argv[-1])
