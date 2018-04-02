package window;

import java.awt.Color;
import java.awt.Cursor;
import java.awt.Font;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import javax.swing.BorderFactory;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;

public class SecPan extends JPanel{
	int width;
	int height;
	
	JPanel panShow;
	public JLabel jlShow;
	public JLabel jlCap;
	public JLabel jlRecd;
	JLabel jlExit;
	
	SecPan(int width, int height) {
		this.width = width;
		this.height = height;
		this.setBounds(0, 0, width, height);
		this.setBorder(BorderFactory.createLineBorder(Color.red));	
		this.setLayout(null);		
		this.setBackground(new Color(103, 188, 255));
		
		initPanShow();
		initLbCap();
		initLbRecd();
		initLbExit();
		this.setVisible(true);
	}
	public void initPanShow() {
		panShow = new JPanel();
		panShow.setBounds(0, 0, width, 480);
		panShow.setLayout(null);
		panShow.setBorder(BorderFactory.createLineBorder(Color.gray));
		panShow.setBackground(new Color(103, 188, 255));
		
		jlShow = new JLabel();
		jlShow.setBounds(70, 0, 640, 480);
		jlShow.setOpaque(true);
		//jlShow.setBackground(Color.green);
		//jlShow.setBorder(BorderFactory.createLineBorder(Color.red));
		
		panShow.add(jlShow);
		this.add(panShow);
	}
	public void initLbCap() {
		jlCap = new JLabel("ÅÄ ÕÕ");
		jlCap.setBounds(80, 500, 100, 40);
		jlCap.setOpaque(true);
		jlCap.setBackground(Color.red);
		jlCap.setHorizontalAlignment(JLabel.CENTER);
		jlCap.setFont(new Font("Î¢ÈíÑÅºÚ", Font.BOLD, 25));
		jlCap.setForeground(new Color(255, 255, 255));
		jlCap.addMouseListener(new MouseListener() {
			
			@Override
			public void mouseReleased(MouseEvent e) {
				
			}
			
			@Override
			public void mousePressed(MouseEvent e) {
				
			}
			
			@Override
			public void mouseExited(MouseEvent e) {
				SecPan.this.setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
			}
			
			@Override
			public void mouseEntered(MouseEvent e) {
				SecPan.this.setCursor(new Cursor(Cursor.HAND_CURSOR));
			}
			
			@Override
			public void mouseClicked(MouseEvent e) {
				
			}
		});
		
		this.add(jlCap);
	}
	public void initLbRecd() {
		jlRecd = new JLabel("Â¼ Ïñ");
		jlRecd.setBounds(300, 500, 100, 40);
		jlRecd.setOpaque(true);
		jlRecd.setBackground(Color.yellow);
		jlRecd.setHorizontalAlignment(JLabel.CENTER);
		jlRecd.setFont(new Font("Î¢ÈíÑÅºÚ", Font.BOLD, 25));
		jlRecd.setForeground(new Color(255, 255, 255));
		jlRecd.addMouseListener(new MouseListener() {
			
			@Override
			public void mouseReleased(MouseEvent e) {
				
			}
			
			@Override
			public void mousePressed(MouseEvent e) {
				
			}
			
			@Override
			public void mouseExited(MouseEvent e) {
				SecPan.this.setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
			}
			
			@Override
			public void mouseEntered(MouseEvent e) {
				SecPan.this.setCursor(new Cursor(Cursor.HAND_CURSOR));
			}
			
			@Override
			public void mouseClicked(MouseEvent e) {
				
			}
		});
		
		this.add(jlRecd);
	}
	public void initLbExit() {
		jlExit = new JLabel("ÍË ³ö");
		jlExit.setBounds(550, 500, 100, 40);
		jlExit.setOpaque(true);
		jlExit.setBackground(new Color(101, 200, 58));
		jlExit.setHorizontalAlignment(JLabel.CENTER);
		jlExit.setFont(new Font("Î¢ÈíÑÅºÚ", Font.BOLD, 25));
		jlExit.setForeground(new Color(255, 255, 255));
		jlExit.addMouseListener(new MouseListener() {
			
			@Override
			public void mouseReleased(MouseEvent e) {
				
			}
			
			@Override
			public void mousePressed(MouseEvent e) {
				
			}
			
			@Override
			public void mouseExited(MouseEvent e) {
				SecPan.this.setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
			}
			
			@Override
			public void mouseEntered(MouseEvent e) {
				SecPan.this.setCursor(new Cursor(Cursor.HAND_CURSOR));
			}
			
			@Override
			public void mouseClicked(MouseEvent e) {
				if(JOptionPane.YES_OPTION 
					== JOptionPane.showConfirmDialog(SecPan.this, "ÄãÈ·¶¨ÍË³ö¿Í»§¶ËÂð£¿", "ÌáÊ¾", JOptionPane.YES_NO_OPTION)) {
					System.exit(0);
				}else {
					
				}
			}
		});
		
		this.add(jlExit);
	}
//	public static void main(String[] args) {
//		JFrame jf = new JFrame();
//		jf.setBounds(200, 100, 800, 600);
//		jf.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
//		jf.setLayout(null);
//		
//		jf.getContentPane().setLayout(null);
//		jf.getContentPane().add(new SecPan(800, 600));
//		jf.setVisible(true);
//	}

}
