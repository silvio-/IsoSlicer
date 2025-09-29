object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'IsoSlicer'
  ClientHeight = 680
  ClientWidth = 1360
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clBlack
  Font.Height = -12
  Font.Name = 'Tahoma'
  Font.Style = []
  OnClose = FormClose
  TextHeight = 14
  object Image1: TImage
    Left = 346
    Top = 8
    Width = 1000
    Height = 640
    Constraints.MaxHeight = 640
    Constraints.MaxWidth = 1000
  end
  object Label7: TLabel
    Left = 8
    Top = 656
    Width = 126
    Height = 16
    Caption = 'No NURBS is loaded.'
    Color = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentColor = False
    ParentFont = False
  end
  object Label58: TLabel
    Left = 8
    Top = 609
    Width = 4
    Height = 16
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label15: TLabel
    Left = 4
    Top = 635
    Width = 4
    Height = 16
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object PageControl1: TPageControl
    Left = 8
    Top = 8
    Width = 336
    Height = 593
    ActivePage = TabSheet1
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -12
    Font.Name = 'Tahoma'
    Font.Style = []
    Images = ImageList1
    ParentFont = False
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = 'Camera'
      object Label1: TLabel
        Left = 100
        Top = 16
        Width = 41
        Height = 16
        Caption = 'Focus:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clGreen
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label2: TLabel
        Left = 70
        Top = 38
        Width = 71
        Height = 16
        Caption = 'Target (T):'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clGreen
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label5: TLabel
        Left = 120
        Top = 59
        Width = 21
        Height = 16
        Caption = 'Up:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clGreen
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label6: TLabel
        Left = 89
        Top = 81
        Width = 52
        Height = 16
        Caption = 'd,hx,hy:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clGreen
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label36: TLabel
        Left = 39
        Top = 126
        Width = 100
        Height = 16
        Caption = 'Light Intensity:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label37: TLabel
        Left = -1
        Top = 148
        Width = 141
        Height = 16
        Caption = 'Light Source Position:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label38: TLabel
        Left = 119
        Top = 171
        Width = 22
        Height = 16
        Caption = 'Od:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label39: TLabel
        Left = 20
        Top = 193
        Width = 122
        Height = 16
        Caption = 'Ambient Intensity:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label40: TLabel
        Left = 73
        Top = 217
        Width = 68
        Height = 16
        Caption = 'Ka, Kd, Ks:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Edit1: TEdit
        Left = 145
        Top = 16
        Width = 50
        Height = 22
        TabOrder = 0
        Text = '20'
      end
      object Edit2: TEdit
        Left = 195
        Top = 16
        Width = 52
        Height = 22
        TabOrder = 1
        Text = '20'
      end
      object Edit3: TEdit
        Left = 247
        Top = 16
        Width = 50
        Height = 22
        TabOrder = 2
        Text = '20'
      end
      object Edit4: TEdit
        Left = 145
        Top = 38
        Width = 50
        Height = 22
        TabOrder = 3
        Text = '0'
      end
      object Edit5: TEdit
        Left = 196
        Top = 38
        Width = 50
        Height = 22
        TabOrder = 4
        Text = '1'
      end
      object Edit6: TEdit
        Left = 247
        Top = 38
        Width = 50
        Height = 22
        TabOrder = 5
        Text = '5'
      end
      object Edit7: TEdit
        Left = 145
        Top = 60
        Width = 50
        Height = 22
        TabOrder = 6
        Text = '0'
      end
      object Edit8: TEdit
        Left = 196
        Top = 58
        Width = 50
        Height = 22
        TabOrder = 7
        Text = '0'
      end
      object Edit9: TEdit
        Left = 247
        Top = 59
        Width = 50
        Height = 22
        TabOrder = 8
        Text = '1'
      end
      object Edit10: TEdit
        Left = 145
        Top = 81
        Width = 50
        Height = 22
        TabOrder = 9
        Text = '1'
      end
      object Edit11: TEdit
        Left = 196
        Top = 81
        Width = 50
        Height = 22
        TabOrder = 10
        Text = '0.5'
      end
      object Edit12: TEdit
        Left = 247
        Top = 81
        Width = 50
        Height = 22
        TabOrder = 11
        Text = '0.32'
      end
      object Button1: TButton
        Left = 73
        Top = 392
        Width = 208
        Height = 25
        Caption = 'Load Texture (.BMP)'
        TabOrder = 12
        OnClick = Button1Click
      end
      object Edit37: TEdit
        Left = 143
        Top = 124
        Width = 50
        Height = 22
        TabOrder = 13
        Text = '255'
      end
      object Edit38: TEdit
        Left = 195
        Top = 124
        Width = 50
        Height = 22
        TabOrder = 14
        Text = '255'
      end
      object Edit39: TEdit
        Left = 247
        Top = 124
        Width = 50
        Height = 22
        TabOrder = 15
        Text = '255'
      end
      object Edit40: TEdit
        Left = 143
        Top = 146
        Width = 50
        Height = 22
        TabOrder = 16
        Text = '20'
      end
      object Edit41: TEdit
        Left = 195
        Top = 146
        Width = 50
        Height = 22
        TabOrder = 17
        Text = '20'
      end
      object Edit42: TEdit
        Left = 247
        Top = 146
        Width = 50
        Height = 22
        TabOrder = 18
        Text = '20'
      end
      object Edit43: TEdit
        Left = 143
        Top = 169
        Width = 50
        Height = 22
        TabOrder = 19
        Text = '250'
      end
      object Edit44: TEdit
        Left = 195
        Top = 169
        Width = 50
        Height = 22
        TabOrder = 20
        Text = '150'
      end
      object Edit45: TEdit
        Left = 247
        Top = 169
        Width = 50
        Height = 22
        TabOrder = 21
        Text = '150'
      end
      object Edit46: TEdit
        Left = 143
        Top = 192
        Width = 50
        Height = 22
        TabOrder = 22
        Text = '255'
      end
      object Edit47: TEdit
        Left = 195
        Top = 192
        Width = 50
        Height = 22
        TabOrder = 23
        Text = '255'
      end
      object Edit48: TEdit
        Left = 247
        Top = 192
        Width = 50
        Height = 22
        TabOrder = 24
        Text = '255'
      end
      object Edit49: TEdit
        Left = 143
        Top = 216
        Width = 50
        Height = 22
        TabOrder = 25
        Text = '0.5'
      end
      object Edit50: TEdit
        Left = 195
        Top = 215
        Width = 50
        Height = 22
        TabOrder = 26
        Text = '0.5'
      end
      object Edit51: TEdit
        Left = 247
        Top = 215
        Width = 50
        Height = 22
        TabOrder = 27
        Text = '0.3'
      end
      object RadioGroup3: TRadioGroup
        Left = 73
        Top = 258
        Width = 208
        Height = 105
        Caption = 'Shading'
        ItemIndex = 0
        Items.Strings = (
          'Phong Shading'
          'Phong Shading with Texture')
        TabOrder = 28
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'NURBS'
      ImageIndex = 1
      object Button2: TButton
        Left = 1
        Top = -1
        Width = 99
        Height = 25
        Caption = 'Load a NURBS'
        TabOrder = 0
        OnClick = Button2Click
      end
      object Button3: TButton
        Left = 172
        Top = -1
        Width = 74
        Height = 25
        Caption = 'Control mesh'
        TabOrder = 1
        OnClick = Button3Click
      end
      object Panel1: TPanel
        Left = 1
        Top = 26
        Width = 323
        Height = 273
        TabOrder = 2
        object Label8: TLabel
          Left = 32
          Top = 8
          Width = 171
          Height = 19
          Caption = 'Isoparametric curves'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clFuchsia
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label9: TLabel
          Left = 89
          Top = 75
          Width = 51
          Height = 14
          Caption = 'isovalue='
        end
        object Label41: TLabel
          Left = 4
          Top = 200
          Width = 105
          Height = 14
          Caption = '# Phong triangles :'
        end
        object Label42: TLabel
          Left = 3
          Top = 138
          Width = 152
          Height = 19
          Caption = 'Rendering NURBS:'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clGreen
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label3: TLabel
          Left = 221
          Top = 101
          Width = 33
          Height = 14
          Caption = 'Min Z:'
        end
        object Label4: TLabel
          Left = 220
          Top = 118
          Width = 36
          Height = 14
          Caption = 'Max Z:'
        end
        object Label13: TLabel
          Left = 205
          Top = 100
          Width = 12
          Height = 29
          Caption = '{'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -24
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Label14: TLabel
          Left = 128
          Top = 108
          Width = 76
          Height = 14
          Caption = 'Control mesh:'
        end
        object RadioGroup1: TRadioGroup
          Left = 7
          Top = 28
          Width = 78
          Height = 69
          Caption = 'Parameter:'
          ItemIndex = 1
          Items.Strings = (
            'u'
            'v')
          TabOrder = 0
        end
        object Edit13: TEdit
          Left = 136
          Top = 71
          Width = 52
          Height = 22
          Alignment = taRightJustify
          TabOrder = 1
          Text = '0'
        end
        object Button4: TButton
          Left = 90
          Top = 33
          Width = 97
          Height = 25
          Caption = 'Single isocurve'
          TabOrder = 2
          OnClick = Button4Click
        end
        object Button5: TButton
          Left = 193
          Top = 32
          Width = 122
          Height = 33
          Caption = 'All Isocurves (single)'
          TabOrder = 3
          OnClick = Button5Click
        end
        object CheckBox1: TCheckBox
          Left = 7
          Top = 105
          Width = 87
          Height = 17
          Caption = 'w/ Ctrl points'
          TabOrder = 4
        end
        object Button21: TButton
          Left = 4
          Top = 163
          Width = 100
          Height = 25
          Caption = 'Single NURBS'
          TabOrder = 5
          OnClick = Button21Click
        end
        object Edit52: TEdit
          Left = 100
          Top = 196
          Width = 71
          Height = 22
          Alignment = taRightJustify
          TabOrder = 6
          Text = '30'
        end
        object CheckBox2: TCheckBox
          Left = 5
          Top = 245
          Width = 284
          Height = 22
          Caption = 'Render plane at Z=height (given below)'
          TabOrder = 7
        end
        object Button26: TButton
          Left = 127
          Top = 163
          Width = 109
          Height = 25
          Caption = 'Multiple NURBS'
          TabOrder = 8
          OnClick = Button26Click
        end
        object CheckBox8: TCheckBox
          Left = 5
          Top = 222
          Width = 97
          Height = 17
          Caption = 'Without floor'
          TabOrder = 9
        end
        object Button9: TButton
          Left = 193
          Top = 68
          Width = 122
          Height = 30
          Caption = 'All Isocurves (multi)'
          TabOrder = 10
          OnClick = Button9Click
        end
        object CheckBox4: TCheckBox
          Left = 120
          Top = 222
          Width = 188
          Height = 17
          Caption = 'Render over previous image'
          TabOrder = 11
        end
      end
      object Button6: TButton
        Left = 247
        Top = -1
        Width = 73
        Height = 25
        Caption = 'Save .tex'
        TabOrder = 3
      end
      object Button7: TButton
        Left = 101
        Top = 0
        Width = 70
        Height = 24
        Caption = 'Clear scene'
        TabOrder = 4
        OnClick = Button7Click
      end
      object Panel2: TPanel
        Left = 1
        Top = 299
        Width = 324
        Height = 315
        TabOrder = 5
        object Label10: TLabel
          Left = 10
          Top = 1
          Width = 53
          Height = 19
          Caption = 'Slicing'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clFuchsia
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label11: TLabel
          Left = 175
          Top = 86
          Width = 55
          Height = 14
          Caption = 'height Z='
        end
        object Label12: TLabel
          Left = 6
          Top = 22
          Width = 27
          Height = 23
          Caption = 'e ='
          Font.Charset = SYMBOL_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Symbol'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label26: TLabel
          Left = 15
          Top = 134
          Width = 4
          Height = 16
          Color = clWhite
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentColor = False
          ParentFont = False
        end
        object Label44: TLabel
          Left = 11
          Top = 239
          Width = 33
          Height = 13
          Caption = 'Min Z:'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clGreen
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label47: TLabel
          Left = 4
          Top = 45
          Width = 28
          Height = 23
          Caption = 'd ='
          Font.Charset = SYMBOL_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Symbol'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label48: TLabel
          Left = 75
          Top = 55
          Width = 32
          Height = 14
          Caption = 'Prec.:'
        end
        object Label49: TLabel
          Left = 6
          Top = 171
          Width = 111
          Height = 13
          Caption = 'Number of samples:'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clTeal
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label55: TLabel
          Left = 7
          Top = 143
          Width = 112
          Height = 13
          Caption = 'CPU time (milisecs):'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clTeal
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label60: TLabel
          Left = 44
          Top = 199
          Width = 73
          Height = 13
          Caption = 'samples/ms:'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clTeal
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label63: TLabel
          Left = 131
          Top = 240
          Width = 37
          Height = 13
          Caption = 'Max Z:'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clGreen
          Font.Height = -11
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Edit14: TEdit
          Left = 231
          Top = 83
          Width = 84
          Height = 22
          Alignment = taRightJustify
          TabOrder = 0
          Text = '1.0'
        end
        object Button8: TButton
          Left = 6
          Top = 80
          Width = 165
          Height = 25
          Caption = 'Single NURBS - Single layer at'
          TabOrder = 1
          OnClick = Button8Click
        end
        object Edit15: TEdit
          Left = 33
          Top = 26
          Width = 36
          Height = 22
          Alignment = taRightJustify
          TabOrder = 2
          Text = '0.01'
        end
        object CheckBox3: TCheckBox
          Left = 181
          Top = 4
          Width = 59
          Height = 17
          Caption = 'z-buffer'
          TabOrder = 3
        end
        object Edit54: TEdit
          Left = 33
          Top = 48
          Width = 36
          Height = 22
          Alignment = taRightJustify
          TabOrder = 4
          Text = '0.25'
        end
        object Edit55: TEdit
          Left = 103
          Top = 52
          Width = 39
          Height = 22
          TabOrder = 5
          Text = '0.1'
        end
        object Edit56: TEdit
          Left = 120
          Top = 169
          Width = 124
          Height = 22
          Alignment = taRightJustify
          TabOrder = 6
          Text = ' 0'
        end
        object Edit57: TEdit
          Left = 121
          Top = 139
          Width = 123
          Height = 22
          Alignment = taRightJustify
          TabOrder = 7
          Text = '0.00'
        end
        object Button24: TButton
          Left = 6
          Top = 106
          Width = 154
          Height = 25
          Caption = 'Single NURBS - Multilayer'
          TabOrder = 8
          OnClick = Button24Click
        end
        object Button25: TButton
          Left = 162
          Top = 106
          Width = 154
          Height = 25
          Caption = 'Multiple NURBS - Multilayer'
          TabOrder = 9
          OnClick = Button25Click
        end
        object Edit62: TEdit
          Left = 120
          Top = 197
          Width = 124
          Height = 22
          Alignment = taRightJustify
          TabOrder = 10
          Text = '0'
        end
        object Edit63: TEdit
          Left = 47
          Top = 237
          Width = 65
          Height = 22
          TabOrder = 11
          Text = ' 0'
        end
        object Edit64: TEdit
          Left = 172
          Top = 237
          Width = 65
          Height = 22
          TabOrder = 12
          Text = ' 0'
        end
        object CheckBox7: TCheckBox
          Left = 250
          Top = 4
          Width = 59
          Height = 17
          Caption = 'Trim'
          TabOrder = 13
        end
      end
    end
  end
  object OpenDialog1: TOpenDialog
    Left = 688
    Top = 16
  end
  object ImageList1: TImageList
    Left = 712
    Top = 280
  end
  object SaveDialog1: TSaveDialog
    Left = 520
    Top = 184
  end
end
