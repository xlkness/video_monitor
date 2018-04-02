package window;

import java.awt.Color;
import java.awt.Cursor;
import java.awt.Font;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.swing.BorderFactory;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;

import socket.SocketOp;

public class FirPan extends JPanel{
	JPanel colorPan1;
	JPanel colorPan2;
	JPanel colorPan3;
	
	JTextField jtfIp;
	JTextField jtfPort;
	JLabel jlIpTips;
	JLabel jlPortTips;
	
	String defaultIpStr = "I P";
	String defaultPortStr = "P O R T";
	String ipReg = 
			"^((\\d|[1-9]\\d|1\\d\\d|2[0-4]\\d|25[0-5]|[*])\\.){3}(\\d|[1-9]\\d|1\\d\\d|2[0-4]\\d|25[0-5]|[*])$";
	
	JLabel jlCon;
	JLabel jlExit;
	
	int width;
	int height;
	int conFlag = 0;
	
	Socket socket;
	SocketOp socketOp;
	
	String ip = "192.168.99.250";
	int port = 8888;
	int conStatus = 0;
	
	FirPan(int width, int height) {
		this.width = width;
		this.height = height;
		this.setBounds(0, 0, width, height);
		this.setBorder(BorderFactory.createLineBorder(Color.red));	
		this.setLayout(null);
		
		this.setBackground(new Color(86, 212, 50));
		
		initJlcon();
		initjlExit();
		initInputField();
		initColorPan();	
		
		this.setVisible(true);
		
	}
	public void initInputField() {
		jtfIp = new JTextField("I P", 15);
		//jtfIp.setEditable(true);
		jtfIp.setHorizontalAlignment(JTextField.CENTER);
		jtfIp.setEnabled(true);
		jtfIp.setFont(new Font("微软雅黑",Font.BOLD|Font.ITALIC,16));
		jtfIp.setBounds(width / 4, height / 5, width / 3 + width / 8, height / 14);
		this.setFocusable(true);
		jtfIp.addFocusListener(new FocusListener() {
			
			@Override
			public void focusLost(FocusEvent e) {
				if(FirPan.this.jtfIp.getText().equals(""))
					FirPan.this.jtfIp.setText(defaultIpStr);
			}
			
			@Override
			public void focusGained(FocusEvent e) {
				if(FirPan.this.jtfIp.getText().equals(defaultIpStr))
					FirPan.this.jtfIp.setText("");
			}
		});
		jlIpTips = new JLabel();
		jlIpTips.setBounds(width / 4 + width / 15, height / 5 + height / 14, width / 3 + width / 8, height / 14);
		jlIpTips.setFont(new Font("微软雅黑", Font.PLAIN, 14));
		jlIpTips.setForeground(Color.red);
		this.add(jlIpTips);
		
		jtfPort = new JTextField("P O R T", 10);
		//jtfIp.setEditable(true);
		jtfPort.setHorizontalAlignment(JTextField.CENTER);
		jtfPort.setEnabled(true);
		jtfPort.setFont(new Font("微软雅黑",Font.BOLD|Font.ITALIC,16));
		jtfPort.setBounds(width / 4, height / 5 + height / 7, width / 3 + width / 8, height / 14);
		this.setFocusable(true);
		jtfPort.addFocusListener(new FocusListener() {
			
			@Override
			public void focusLost(FocusEvent e) {
				if(FirPan.this.jtfPort.getText().equals(""))
					FirPan.this.jtfPort.setText(defaultPortStr);
			}
			
			@Override
			public void focusGained(FocusEvent e) {
				if(FirPan.this.jtfPort.getText().equals(defaultPortStr))
					FirPan.this.jtfPort.setText("");
			}
		});
		jtfPort.addKeyListener(new KeyListener() {
			
			@Override
			public void keyTyped(KeyEvent e) {
				int keyChar=e.getKeyChar();
				if (keyChar>=KeyEvent.VK_0 && keyChar<=KeyEvent.VK_9) {

				} else {
				e.consume(); 
				}
			}
			
			@Override
			public void keyReleased(KeyEvent e) {
				
			}
			
			@Override
			public void keyPressed(KeyEvent e) {
				
			}
		});

		jlPortTips = new JLabel();
		jlPortTips.setBounds(width / 4 + width / 15, height / 5 + height / 7 + height / 14, width / 3 + width / 8, height / 14);
		jlPortTips.setFont(new Font("微软雅黑", Font.PLAIN, 14));
		jlPortTips.setForeground(Color.red);
		this.add(jlPortTips);		
		
		this.add(jtfIp);
		this.add(jtfPort);
	}
	public void initJlcon() {
		jlCon = new JLabel();
		jlCon.setBounds(width / 4, height - height / 2, width / 3 + width / 8, height / 12);
		//jlCon.setBorder(BorderFactory.createLineBorder(Color.gray));
		jlCon.setFont(new Font("微软雅黑", Font.BOLD, 20));
		jlCon.setHorizontalAlignment(JLabel.CENTER);
		jlCon.setForeground(new Color(255, 255, 255));
		jlCon.setText("连 接 服 务 器");
		jlCon.setBackground(new Color(232, 201, 7));
		jlCon.setOpaque(true);
		jlCon.addMouseListener(new MouseListener() {
			
			@Override
			public void mouseReleased(MouseEvent e) {
				
			}
			
			@Override
			public void mousePressed(MouseEvent e) {
				
			}
			
			@Override
			public void mouseExited(MouseEvent e) {
				FirPan.this.setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
				FirPan.this.jlCon.setBackground(new Color(232, 201, 7));
				FirPan.this.jlCon.setForeground(new Color(255, 255, 255));
			}
			
			@Override
			public void mouseEntered(MouseEvent e) {
				FirPan.this.setCursor(new Cursor(Cursor.HAND_CURSOR));
				FirPan.this.jlCon.setBackground(new Color(255, 255, 255));
				FirPan.this.jlCon.setForeground(new Color(0, 0, 0));
			}
			
			@Override
			public void mouseClicked(MouseEvent e) {
				jlIpTips.setText("");
				jlPortTips.setText("");
				final String ip = FirPan.this.jtfIp.getText();
				String portStr = FirPan.this.jtfPort.getText();
				final int port;
				if(ip.equals(defaultIpStr) || ip.equals("")) {
					jlIpTips.setText("请输入IP地址！");
				}else {
					if(!portStr.equals(defaultPortStr) && !portStr.equals("")) {
						port = Integer.valueOf(portStr);
						Pattern pattern = Pattern.compile(ipReg); 
						Matcher matcher = pattern.matcher(FirPan.this.jtfIp.getText());	
						if(matcher.matches()) {
					    	if(port < 0 || port > 100000) {
					    		jlPortTips.setText("请输入合法端口号(0-100000)！");
					    	}
						    else {
						    	Thread th1 = new Thread() {
						    		public void run() {
						    			socket = new Socket();
						    			try {
											socket.connect(new InetSocketAddress(ip, port), 4000);
											socketOp = new SocketOp(socket);
											conFlag = 1;
										} catch (IOException e) {
											jlIpTips.setText("连接服务器失败！请检查ip/port是否正确或其它原因");
											e.printStackTrace();
										}
//								    	if(1 == socketOp.conStatus) {
//								    		conFlag = 1;
//								    	}else {
//								    		jlIpTips.setText("连接服务器失败！请检查ip/port是否正确或其它原因");
//								    	}
						    		}
						    	};
						    	th1.start();
						    }
						}else {
							jlIpTips.setText("IP地址不合法！");
						}
					}else {
						jlPortTips.setText("请输入端口号(0-100000)！");
					}
				}				
			}
		});
		
		this.add(jlCon);
	}
	
	public void initjlExit() {
		jlExit = new JLabel();
		jlExit.setBounds(width / 4, height - height / 3, width / 3 + width / 8, height / 12);
		//jlExit.setBorder(BorderFactory.createLineBorder(Color.blue));
		jlExit.setBackground(new Color(216, 41, 76));
		jlExit.setFont(new Font("微软雅黑", Font.BOLD, 20));
		jlExit.setHorizontalAlignment(JLabel.CENTER);
		jlExit.setForeground(new Color(255, 255, 255));
		jlExit.setText("退 出 客 户 端");
		jlExit.setOpaque(true);
		jlExit.addMouseListener(new MouseListener() {
			
			@Override
			public void mouseReleased(MouseEvent e) {
				
			}
			
			@Override
			public void mousePressed(MouseEvent e) {
				
			}
			
			@Override
			public void mouseExited(MouseEvent e) {
				FirPan.this.setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
				FirPan.this.jlExit.setBackground(new Color(216, 41, 76));
				FirPan.this.jlExit.setForeground(new Color(255, 255, 255));
			}
			
			@Override
			public void mouseEntered(MouseEvent e) {
				FirPan.this.setCursor(new Cursor(Cursor.HAND_CURSOR));
				FirPan.this.jlExit.setBackground(new Color(255, 255, 255));
				FirPan.this.jlExit.setForeground(new Color(0, 0, 0));
			}
			
			@Override
			public void mouseClicked(MouseEvent e) {
				if(JOptionPane.YES_OPTION 
					== JOptionPane.showConfirmDialog(FirPan.this, "你确定退出客户端吗？", "提示", JOptionPane.YES_NO_OPTION)) {
					System.exit(0);
				}else {
					
				}
			}
		});
		
		this.add(jlExit);
	}
	public void initColorPan() {
		colorPan1 = new JPanel();
		colorPan2 = new JPanel();
		colorPan3 = new JPanel();
		
		colorPan1.setBounds(0, 0, width, height / 3);
		colorPan1.setBackground(new Color(52, 217, 247));
		this.add(colorPan1);
		
		colorPan2.setBounds(0, height / 3 , width, height / 3);
		colorPan2.setBackground(new Color(17, 118, 190));
		this.add(colorPan2);
		
		colorPan3.setBounds(0, height - height / 3 , width, height / 3);
		colorPan3.setBackground(new Color(40, 176, 222));
		this.add(colorPan3);
	}
//	public static void main(String[] args) {
//		JFrame jf = new JFrame();
//		jf.setBounds(200, 100, 800, 600);
//		jf.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
//		jf.setLayout(null);
//		
//		jf.getContentPane().setLayout(null);
//		jf.getContentPane().add(new FirPan(800, 600));
//		jf.setVisible(true);
//	}

}
