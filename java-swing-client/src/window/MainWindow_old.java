package window;

import java.awt.CardLayout;
import java.awt.Color;
import java.awt.Container;
import java.awt.Cursor;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionAdapter;

import javax.swing.BorderFactory;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;

public class MainWindow_old extends JFrame {
	Container 	container;
	CardLayout	card;
	
	JPanel		panInit;
	JLabel		jlConn;
	JDialog		dlg1;
	
	JPanel 		panMain;
	JPanel 		panShow;
	JPanel 		panOpt;
	int 		xOld;
	int 		yOld;

	MainWindow_old() {
		initFrame();	
	}

	public void initFrame() {
		this.setBounds(200, 100, 800, 600);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setUndecorated(true);		
		this.setLayout(null);	
		
		initPanInit();
		initPanMain();
		
		card = new CardLayout(0, 3);
		container = this.getContentPane();
		container.setLayout(card);
		container.add(panInit);
		container.add(panMain);
		
		this.addMouseListener(new MouseAdapter() {
			@Override
			public void mousePressed(MouseEvent e) {
				xOld = e.getX();
				yOld = e.getY();
			}
		});
		this.addMouseMotionListener(new MouseMotionAdapter() {
			@Override
			public void mouseDragged(MouseEvent e) {
				int xOnScreen = e.getXOnScreen();
				int yOnScreen = e.getYOnScreen();
				int xx = xOnScreen - xOld;
				int yy = yOnScreen - yOld;
				MainWindow_old.this.setLocation(xx, yy);
			}
		});
	
		this.setVisible(true);
	}
	public void initPanInit() {
		panInit = new JPanel();
		panInit.setBounds(0, 0, 800, 600);
		panInit.setBorder(BorderFactory.createLineBorder(Color.red));
		panInit.setLayout(null);
		panInit.setBackground(new Color(86, 212, 50));
		
		jlConn = new JLabel();
		jlConn.setBounds(200, 100, 80, 50);
		jlConn.setBorder(BorderFactory.createLineBorder(Color.gray));
		jlConn.setText("连 接");
		jlConn.addMouseListener(new MouseListener() {
			
			@Override
			public void mouseReleased(MouseEvent e) {
				
			}
			
			@Override
			public void mousePressed(MouseEvent e) {
				
			}
			
			@Override
			public void mouseExited(MouseEvent e) {
				panInit.setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
			}
			
			@Override
			public void mouseEntered(MouseEvent e) {
				panInit.setCursor(new Cursor(Cursor.HAND_CURSOR));
			}
			
			@Override
			public void mouseClicked(MouseEvent e) {
//				if(JOptionPane.YES_OPTION 
//					== JOptionPane.showConfirmDialog(MainWindow.this, "你确定退出客户端吗？", "提示", JOptionPane.YES_NO_OPTION)) {
//					System.exit(0);
//				}else {
//					System.out.println("no");
//				}
				
			}
		});
		panInit.add(jlConn);
	}
	public void initPanMain() {
		panMain = new JPanel();
		panMain.setBounds(0, 0, 800, 600);
		panMain.setBorder(BorderFactory.createLineBorder(Color.red));
		panMain.setLayout(null);
		initPanShow();
		initPanOpt();
		panMain.add(panShow);
		panMain.add(panOpt);
		panMain.setBackground(new Color(103, 188, 255));	
	}
	public void initPanShow() {
		panShow = new JPanel();
		panShow.setBounds(75, 20, 650, 500);
		panShow.setOpaque(false);
		panShow.setBorder(BorderFactory.createLineBorder(Color.gray));
		panShow.setBackground(new Color(255, 228, 0));
		panShow.setLayout(null);		
	}

	public void initPanOpt() {
		panOpt = new JPanel();
		panOpt.setBounds(75, 520, 650, 75);
		panOpt.setBorder(BorderFactory.createLineBorder(Color.blue));
		panOpt.setOpaque(false);
		panOpt.setBackground(new Color(86, 212, 50));
		panOpt.setBackground(Color.yellow);
		panOpt.setLayout(null);	
	}

	public static void main(String[] args) {
		new MainWindow_old();
	}

}
